/**
 * MIT License
 *
 * Copyright (c) 2020 R. D. Poor <rdpoor@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// =============================================================================
// Includes

#include "tower.h"

#include "animator.h"
#include "disk.h"
#include "fb.h"
#include "pole.h"
#include <mulib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// Local types and definitions

#define POLE_A 0
#define POLE_B 1
#define POLE_C 2
#define N_POLES 3

#define BUFFER_WIDTH ((N_POLES * POLE_WIDTH) + N_POLES)
#define BUFFER_HEIGHT (CRUISING_ALTITUDE)

typedef struct {
  mu_task_t task;
  uint8_t phase;
} tower_ctx_t;

// =============================================================================
// Local storage

static tower_ctx_t s_tower_ctx;
static char s_backing_buf[BUFFER_WIDTH * BUFFER_HEIGHT];
static char s_display_buf[BUFFER_WIDTH * BUFFER_HEIGHT];
static disk_t s_disks[N_DISKS];
static pole_t s_poles[N_POLES];

// =============================================================================
// Local (forward) declarations

/**
 * @brief This is the function that gets called when the tower task runs.
 */
static void tower_task_fn(void *ctx, void *arg);

/**
 * @brief Move a disk between two poles.
 *
 * Note: we don't know which pole is the source and which is the destination;
 * see find_destination_pole() for that...
 *
 * @return true if there are more moves to make, false otherwise.
 */
static bool move_disk(pole_t *p1, pole_t *p2);

/**
 * @brief Decide which of the two poles is the destination pole.
 *
 * If one pole is empty, it is alwasys the destination.  If both poles have
 * disks, the one with the larger disk is the destination.  If both poles are
 * empty, then the algorithm has terminated and this function returns NULL.
 */
static pole_t *find_destination_pole(pole_t *p1, pole_t *p2);

/**
 * @brief Transfer a disk from src to dst, and start the task to animate the
 * the move.
 */
static void move_disk_aux(pole_t *src, pole_t *dst);

/**
 * @brief Reset the towers and disks.
 */
static void reset(void);

// =============================================================================
// Public code

void tower_init(void) {
  mulib_init();
  mu_button_io_set_callback(NULL);
  mu_task_init(&s_tower_ctx.task, tower_task_fn, &s_tower_ctx, "Tower");
  // initialize the frame buffer
  fb_init(BUFFER_WIDTH, BUFFER_HEIGHT, s_backing_buf, s_display_buf);

  // set up tower and disk positions
  fb_erase();
  reset();

  // Schedule the task.
  mu_sched_task_now(&s_tower_ctx.task);
}

void tower_step(void) {
  mu_sched_step();
}

void tower_draw(void) {
  fb_clear();
  for (int i=0; i<N_POLES; i++) {
    pole_draw(&s_poles[i]);
  }
  for (int i=0; i<N_DISKS; i++) {
    disk_draw(&s_disks[i]);
  }
  fb_flush();
}

// =============================================================================
// Local (static) code

static void tower_task_fn(void *ctx, void *arg) {
  tower_ctx_t *self = (tower_ctx_t *)ctx;
  (void)arg;
  bool running;

  // NOTE: unless the algo has terminated, move_disk() will start the animator
  // task, which will call back to this task upon completion.
  if (self->phase == 0) {
    running = move_disk(&s_poles[POLE_A], &s_poles[POLE_C]);
  } else if (self->phase == 1) {
    running = move_disk(&s_poles[POLE_A], &s_poles[POLE_B]);
  } else /* if (self->phase == 2) */ {
    running = move_disk(&s_poles[POLE_B], &s_poles[POLE_C]);
  }

  // increment phase
  if (self->phase < 2) {
    self->phase += 1;
  } else {
    self->phase = 0;
  }

  if (!running) {
    // Algo completed.  All disks are on POLE_C.  Reset everything and restart
    // after a short delay...
    reset();
    mu_sched_task_in(&s_tower_ctx.task, MU_TIME_MS_TO_DURATION(5000));
  }
}

static bool move_disk(pole_t *p1, pole_t *p2) {
  pole_t *dst;

  // which of the two poles is the destination?
  dst = find_destination_pole(p1, p2);
  if (dst == NULL) {
    return false;   // algorithm has completed
  } else if (dst == p1) {
    move_disk_aux(p2, p1);
  } else {
    move_disk_aux(p1, p2);
  }
  return true;
}

static pole_t *find_destination_pole(pole_t *p1, pole_t *p2) {
  disk_t *d1 = pole_top(p1);
  disk_t *d2 = pole_top(p2);
  if (d1 == NULL && d2 == NULL) {
    // both poles empty: algo is complete.
    return NULL;
  } else if (d1== NULL) {
    // pole 1 is empty: it is the destination
    return p1;
  } else if (d2 == NULL) {
    // pole 2 is empty: it is the destination
    return p2;
  } else {
    // the pole with the larger disk is the destination
    return (disk_get_width(d1) > disk_get_width(d2)) ? p1 : p2;
  }
}

static void move_disk_aux(pole_t *src, pole_t *dst) {
  // Move disk from src to dst
  disk_t *disk = pole_pop(src);
  pole_push(dst, disk);

  // Start animating the move.  call back to this task on completion.
  mu_task_t *animator = animator_init(disk,
                                pole_top_x(dst),
                                pole_top_y(dst),
                                &s_tower_ctx.task);
  mu_sched_task_now(animator);
}

static void reset(void) {
  s_tower_ctx.phase = 0;

  // Initialize all poles
  for (int i=0; i<N_POLES; i++) {
    uint8_t xpos = i * (POLE_WIDTH + 1) + POLE_WIDTH/2 + 1;
    pole_init(&s_poles[i], xpos);
  }

  // Initialize each disk, push them onto POLE_A (largest first), and set their
  // initial coordinates.
  pole_t *pole = &s_poles[POLE_A];
  for (int i=N_DISKS-1; i>=0; i--) {
    disk_t *disk = &s_disks[i];
    uint8_t width = i*2 + 1;
    disk_init(disk, width);
    pole_push(pole, disk);
    disk_set_position(disk, pole_top_x(pole), pole_top_y(pole));
  }
}
