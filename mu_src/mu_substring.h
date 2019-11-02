#ifndef SUBSTRING_H_
#define SUBSTRING_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// types and definitions

// mu_substring_t is a reference to a mu_substring of a larger string
typedef struct {
  const char *src;
  size_t start;
  size_t length;
} mu_substring_t;

// =============================================================================
// declarations

// Initialize a mu_substring_t object with a reference to an underlying string
mu_substring_t *mu_substring_init(mu_substring_t *substr,
                                  const char *str,
                                  int start,
                                  size_t length);

// Return a reference to the start of the mu_substring
const char *mu_substring_data(mu_substring_t *substr);

// Return the length of the mu_substring
size_t mu_substring_length(mu_substring_t *substr);

// Compare a mu_substring to a string
int mu_substring_cmp(mu_substring_t *substr, const char *s);

// Return true if the mu_substring is equal to another string
bool mu_substring_eq(mu_substring_t *substr, const char *s);

// Copy up to dst_length characters from a mu_substring into dst
char *mu_substring_extract(mu_substring_t *substr, char *dst, int dst_length);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SUBSTRING_H_ */
