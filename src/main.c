#include <stdio.h>
#include "util/generator.h"

int main(void) {
    double x, y;
    struct MT19937 gen;
    mt19937_initialize(&gen, 100);
    for (int i = 0; i < 10; ++i) {
        generator_vct(&gen, 1, &x, &y);
        printf("x: %lf y: %lf r: %lf\n", x, y, x * x + y * y);
    }
    return 0;
}
