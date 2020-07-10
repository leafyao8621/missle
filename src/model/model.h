#ifndef _MODEL_H_
#define _MODEL_H_

#include "../util/generator.h"

#define STAT_ONGOING 0
#define STAT_SUCCESS 1
#define STAT_FAILURE 2

struct Entity {
    double acc_x, acc_y;
    double vel_x, vel_y;
    double loc_x, loc_y;
    double max_acc, max_acc_sq, max_spd, max_spd_sq;
};

struct Model {
    char stat;
    double width, height, blast_radius, blast_rasius_sq;
    struct MT19937 gen;
    struct Entity target, missle;
};

int model_initialize(struct Model *m,
                     double dist,
                     double blast_radius,
                     unsigned seed,
                     double target_max_acc,
                     double target_max_spd,
                     double missle_max_acc,
                     double missle_max_spd);
int model_update(struct Model *m);
int model_log(struct Model *m, FILE *fout);

#endif
