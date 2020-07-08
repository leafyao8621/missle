#include <stdio.h>
#include <math.h>
#include "../util/generator.h"
#include "model.h"

static inline void entity_update_acc(struct Entity *e, struct MT19937 *gen) {
    generator_vct(gen, e->max_acc, &e->acc_x, &e->acc_y);
}

static inline void entity_update_vec(struct Entity *e) {
    e->vel_x += e->acc_x;
    e->vel_y += e->acc_y;
    double spd_sq = e->vel_x * e->vel_x + e->vel_y * e->vel_y;
    if (spd_sq > e->max_spd_sq) {
        double rat = sqrt(e->max_spd_sq / spd_sq);
        e->vel_x /= rat;
        e->vel_y /= rat;
    }
}

static inline void entity_update_loc(struct Entity *e) {
    e->loc_x += e->vel_x;
    e->loc_y += e->vel_y;
}

int model_initialize(struct Model *m,
                     double width,
                     double height,
                     unsigned seed,
                     double target_max_acc,
                     double target_max_spd,
                     double missle_max_acc,
                     double missle_max_spd) {
    if (!m) {
        return 1;
    }
    if (width <= 0) {
        return 2;
    }
    m->width = width;
    if (height <= 0) {
        return 3;
    }
    m->height = height;
    mt19937_initialize(&m->gen, seed);
    if (target_max_acc <= 0) {
        return 4;
    }
    m->target.max_acc = target_max_acc;
    if (target_max_spd <= 0) {
        return 5;
    }
    m->target.max_spd = target_max_spd;
    m->target.max_spd_sq = target_max_spd * target_max_spd;
    if (missle_max_acc <= 0) {
        return 6;
    }
    m->missle.max_acc = missle_max_acc;
    if (missle_max_spd <= 0) {
        return 7;
    }
    m->missle.max_spd = missle_max_spd;
    m->missle.max_spd_sq = missle_max_spd * missle_max_spd;
    return 0;
}
