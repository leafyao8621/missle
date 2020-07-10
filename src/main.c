#include <stdio.h>
#include "model/model.h"

int main(void) {
    struct Model m;
    model_initialize(&m, 10, 5, 10, 1, 3, 1, 3);
    for (int i = 0; i < 10; ++i) {
        model_update(&m);
        model_log(&m, stdout);
    }
    return 0;
}
