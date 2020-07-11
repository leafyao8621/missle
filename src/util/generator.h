#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "mt19937.h"

int generator_vct(struct MT19937 *gen, double r, double *x, double *y);
int generator_unif(struct MT19937 *gen, double low, double high, double *out);

#endif
