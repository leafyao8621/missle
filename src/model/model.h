#ifndef _MODEL_H_
#define _MODEL_H_

#include "../util/generator.h"

struct Entity {
    double acc_x, acc_y;
    double vel_x, vel_y;
    double loc_x, loc_y;
    double max_acc, max_spd, max_spd_sq;
};

struct Model {
    double width, height;
    struct MT19937 gen;
    struct Entity target, missle;
};

int model_initialize(struct Model *m,
                     double width,
                     double height,
                     unsigned seed,
                     double target_max_acc,
                     double target_max_spd,
                     double missle_max_acc,
                     double missle_max_spd);

#endif
