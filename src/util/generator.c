#include <math.h>
#include "generator.h"

#define PI 3.14159265358979323846

int generator_vct(struct MT19937 *gen, double r, double *x, double *y) {
    if (!gen) {
        return 1;
    }
    double rad = mt19937_gen(gen) / (double)0xffffffffu * PI * 2;
    *x = r * cos(rad);
    *y = r * sin(rad);
    return 0;
}

int generator_unif(struct MT19937 *gen, double low, double high, double *out) {
    if (!gen) {
        return 1;
    }
    *out = mt19937_gen(gen) / (double)0xffffffffu * (high - low) + low;
    return 0;
}
