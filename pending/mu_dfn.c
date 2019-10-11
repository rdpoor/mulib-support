/**
 * mu_dfn: Deferrable Functions
 */

typedef struct mu_dfn;

// Ths signature of a deferrable function.
typedef struct mu_dfn *(*mu_deferrable_fn)(struct *mu_dfn, void *arg);

typedef struct mu_dfn {
  mu_deferrable_fn fn;
} mu_dfn_t;

mu_dfn_t *mu_dfn_call(mu_dfn_t *dfn, void *arg);


mu_dfn_t *mu_dfn_call(mu_dfn_t *dfn, void *arg) {
  return (dfn && dfn->fn != NULL) ? dfn->fn(dfn, arg) : NULL;
}

// lets have fun

typedef struct {
  mu_dfn_t fn;
  int n;
} integers_dfn_t;

mu_dfn_t *integers_fn(integers_dfn_t *dfn, void *arg) {
  
}
mu_dfn_t *integers(integers_dfn_t *dfn, int initial) {
  dfn->fn = integers_fn;
  dfn->n = initial;
  return dfn;
}
