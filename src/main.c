#include "engine/engine.h"

int main(void) {
    struct Engine engine;
    double prob;
    engine_initialize(&engine, 12, 1000000, 100, 10, 3, 10, 10, 1, 3, 1, 3);
    engine_run(&engine, &prob, 0, 0);
    printf("prob: %lf\n", prob);
    return 0;
}
