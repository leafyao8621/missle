#include "engine/engine.h"

int main(void) {
    struct Engine engine;
    double prob;
    char success;
    engine_initialize(&engine, 12, 100, 100, 10, 3, 10, 10, 1, 3, 1, 3);
    engine_run(&engine, &prob, 1, stdout);
    printf("prob: %lf\n", prob);
    return 0;
}
