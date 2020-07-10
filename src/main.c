#include <stdio.h>
#include "model/model.h"

int main(void) {
    struct Model m;
    model_initialize(&m, 10, 0.1, 10, 1, 3, 1, 3);
    for (int i = 0; i < 100; ++i) {
        model_update(&m);
        model_log(&m, stdout);
    }
    return 0;
}
