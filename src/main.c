#include "engine/engine.h"
#include "model/model.h"
int main(void) {
    struct Engine engine;
    char success;
    engine_initialize(&engine, 12, 100, 100, 10, 2, 10, 10, 1, 3, 1, 3);
    int ret = engine_one_iter(&engine, &success, 0, 0);
    model_log(&engine.model, stdout);
    printf("ret: %d success: %hhd\n", ret, success);
    ret = engine_one_iter(&engine, &success, 0, 0);
    model_log(&engine.model, stdout);
    printf("ret: %d success: %hhd\n", ret, success);
    return 0;
}
