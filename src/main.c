#include "engine/engine.h"

int main(void) {
    struct Engine engine;
    char success;
    engine_initialize(&engine, 12, 100, 100, 10, 1, 10, 10, 1, 3, 1, 3);
    engine_one_iter(&engine, &success, 1, stdout);
    printf("success %hhd\n", success);
    return 0;
}
