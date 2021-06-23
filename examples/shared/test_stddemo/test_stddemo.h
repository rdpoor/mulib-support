/*
 * test_stddemo.h
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 */


#ifndef _TEST_STDDEMO_H_
#define _TEST_STDDEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Includes

// =============================================================================
// Types and definitions

// =============================================================================
// Declarations
void set_led(int on);
int get_led(void);

void test_stddemo_x(int b);

void test_stddemo_init(void);

void test_stddemo_step(void);

#ifdef __cplusplus
}
#endif

#endif // _TEST_STDDEMO_H_
