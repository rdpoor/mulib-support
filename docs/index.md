# Welcome to `mulib`
`mulib` is curated collection of software modules in C, written from the ground up for microcontrollers and other resource-constrained environments,
that makes writing embedded applications simpler, faster and more reliable.

Some of the features you'll find in `mulib`:

* a fast and lithe run-to-completion scheduler
* low-overhead timing and event handling
* advanced in-place string handling
* an in-memory publish / subscribe system
* flexible logging
* ... and a variety of other useful modules

## `mulib` is a more of a framework and less of an operating system
`mulib` does not call itself an "operating system", specifically because it doesn't try to be one.  
Like an operating system, `mulib` does have a scheduler that can control when tasks are run, but 
unlike an operating system, `mulib` does not insist on taking over your microcontroller: your code
remains in control.  

## `mulib` has a clear design philosophy

`mulib` keeps the following design goals in mind (even if some of the goals are more aspirational than factual):

* **Self-contained, pure C code:** With minimal dependencies on external libraries, `mulib` "just works" with 
  almost any modern C compiler.
* **No malloc. Ever**: All of mulib's modules support user-allocated data structures.  Of course, if you want
  to use malloc and free for the data structures, you're free to do so.
* **Super portable:** One concise mu_ports.c file defines mulib's interface to your target system's environment.
* **Fast and Dangerous:** Because `mulib` is written for embedded applications, if favors minimizing time and 
  code space over argument validation.
* **Two-file implementation:** Whenever practical, each module is embodied in one header file and one C file,
  allowing you to cherry pick the functions you want without bringing in all of `mulib`.
* **Well-documented:** API documentation is driven by documentation strings in the header files.
* **Well-tested:** Comprehensive unit tests validate the core API.
* **Low threshold, high ceiling:** A suite of demo programs take you from the simplest "blink an LED" example 
  to asynchronous multi-tasking applications.
* **Yours to use:** The entirety of `mulib` is covered under the MIT open source license.

## A short, complete `mulib` example

```c
// =============================================================================
// includes

#include "mulib.h"
#include <definitions.h>  // specific to MPLAB X
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

#define BLINKY_VERSION "1.0.0"

#define EVENT_QUEUE_CAPACITY 10    // up to 10 tasks may be queued
#define ISR_Q_CAPACITY 8           // must be a power of two

// =========
// button_task definitions

typedef struct {
  mu_task_t *task;
  mu_sched_t *sched;
} button_ctx_t;

// =========
// led_task definitions

#define LED_ON_TIME_MS 5
#define LED_OFF_TIME_MS 995

typedef enum { LED_OFF, LED_ON } led_state_t;

typedef struct {
  led_state_t state;
} led_ctx_t;

// =============================================================================
// local (forward) declarations

static void blinky_init(void);
static void blinky_step(void);

static mu_task_t *button_task_init(mu_task_t *button_task,
                                   button_ctx_t *button_ctx,
                                   mu_sched_t *sched);
static void *button_task_fn(void *self, void *arg);
static void button_cb(uintptr_t context);

static void *led_task_fn(void *self, void *arg);

// =============================================================================
// local storage

// the scheduler object
static mu_sched_t s_sched;

// the backing store for scheduled events
static mu_sched_event_t s_event_queue[EVENT_QUEUE_CAPACITY];

// the queue to hold tasks queued from interrupt level and its backing store
static mu_spscq_t s_isr_queue;
static mu_spscq_item_t s_isr_queue_items[ISR_Q_CAPACITY];

// the Button task and context
static mu_task_t s_button_task;
static button_ctx_t s_button_ctx;

// the LED task and context
static mu_task_t s_led_task;
static led_ctx_t s_led_ctx;

// =============================================================================
// Main loop here

int main() {
  SYS_Initialize(NULL);  // Specific to MPLAB X
  blinky_init();  
  while(1) {
    blinky_step();
  }
  // doesn't ever arrive here...
}

static void blinky_init() {
  printf("\n\n# ===========\n");
  printf("# blinky %s: see https://github.com/rdpoor/mulib\n", BLINKY_VERSION);

  // initialize the port-specific interface
  mu_port_init();

  // set up the isr queue and the scheduler
  mu_spscq_init(&s_isr_queue, s_isr_queue_items, ISR_Q_CAPACITY);
  mu_sched_init(&s_sched, s_event_queue, EVENT_QUEUE_CAPACITY, &s_isr_queue);

  // initialize tasks
  led_task_init(&s_led_task, &s_led_ctx);
  button_task_init(&s_button_task, &s_button_ctx, &s_sched);

  // schedule the initial call to the LED task
  mu_sched_task_now(&s_sched, &s_led_task);
}

static void blinky_step() {
  mu_sched_step(&s_sched);
}

// =======================================
// button task code

mu_task_t *button_task_init(mu_task_t *button_task,
                            button_ctx_t *button_ctx,
                            mu_sched_t *sched) {
  // Register the button_cb function to be called upon an EIC interrupt on
  // the user button.  Register the callback with a reference to button_ctx
  // so we can access the task and scheduler objects from within the interrupt.
  button_ctx->task = button_task;
  button_ctx->sched = sched;
  EIC_CallbackRegister(EIC_PIN_15 , button_cb, (uintptr_t)button_ctx);
  EIC_InterruptEnable(EIC_PIN_15);

  mu_task_init(button_task, button_task_fn, NULL, "Button Interrupt");
  return button_task;
}

static void *button_task_fn(void *ctx, void *arg) {
  // a task is called with its context (unused here) as the first argument 
  // and the scheduler as the second.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_time_t now = mu_sched_get_current_time(sched);

  printf("button pressed at %lu\r\n", now);
  return NULL;
}

// button_cb is triggered when a button press generates an interrupt.
// From interrupt level, schedule the button task upon leaving interrupt level.
static void button_cb(uintptr_t context) {
  button_ctx_t *button_ctx = (button_ctx_t *)context;
  mu_sched_task_from_isr(button_ctx->sched, button_ctx->task);
}

// =======================================
// LED task code

static mu_task_t *led_task_init(mu_task_t *led_task, led_ctx_t *led_ctx) {
  led_ctx->state = LED_OFF;
  mu_task_init(led_task, led_task_fn, led_ctx, "LED");
  return led_task;
}

static void *led_task_fn(void *ctx, void *arg) {
  // "context" is passed as the first argument, scheduler is second
  led_ctx_t *led_ctx = (led_ctx_t *)ctx;
  mu_sched_t *sched = (mu_sched_t *)arg;

  if (led_ctx->state == LED_OFF) {
    LED_On();                          // MPLAB X specific
    led_ctx->state = LED_ON;
    mu_sched_reschedule_in(sched, MU_TIME_MS_TO_DURATION(LED_ON_TIME_MS));

  } else {
    LED_Off();                         // MPLAB X specific
    led_ctx->state = LED_OFF;
    mu_sched_reschedule_in(sched, MU_TIME_MS_TO_DURATION(LED_OFF_TIME_MS));
  }

  return NULL;
}

```
