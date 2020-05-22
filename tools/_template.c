/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
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
// includes

#include "mu_template.h"

// =============================================================================
// local types and definitions

#define MU_TEMPLATE_DEBUG (0)

typedef enum {
  MU_TEMPLATE_STATE_0,
} mu_template_state_t;

// =============================================================================
// local (forward) declarations

mu_template_state_t get_state(mu_template_t *template);
mu_template_t *set_state(mu_template_t *template, mu_template_state_t state);

// =============================================================================
// local storage

static mu_template_t s_template;

// =============================================================================
// public code

mu_template_t *mu_template_init() {
  return template_reset(&s_template);
}

// =============================================================================
// local (static) code

static mu_template_t *template_reset(template_t *template) {
  template_set_state(MU_TEMPLATE_STATE_0);
  return template;
}

mu_template_state_t get_state(mu_template_t *template) {
  return template->state;
}

mu_template_t *set_state(mu_template_t *template, mu_template_state_t state) {
  template->state = state;
  return template;
}
