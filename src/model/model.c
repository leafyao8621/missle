#include <stdio.h>
#include <math.h>
#include "../util/generator.h"
#include "model.h"

static inline void entity_update_acc(struct Entity *e, struct MT19937 *gen) {
    generator_vct(gen, e->max_acc, &e->acc_x, &e->acc_y);
}

static inline void entity_chase_acc(struct Entity *m, struct Entity *t) {
    double diff_x = t->loc_x - m->loc_x;
    double diff_y = t->loc_y - m->loc_y;
    double mag_sq = diff_x * diff_x + diff_y * diff_y;
    double rat = sqrt(m->max_spd_sq / mag_sq);
    diff_x *= rat;
    diff_y *= rat;
    double acc_x = diff_x - m->vel_x;
    double acc_y = diff_y - m->vel_y;
    mag_sq = acc_x * acc_x + acc_y * acc_y;
    if (mag_sq > m->max_acc_sq) {
        rat = sqrt(m->max_acc_sq / mag_sq);
        acc_x *= rat;
        acc_y *= rat;
    }
    m->acc_x = acc_x;
    m->acc_y = acc_y;
}

static inline void entity_update_vel(struct Entity *e) {
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

static inline double entity_dist_sq(struct Entity *a, struct Entity *b) {
    double xdiff = a->loc_x - b->loc_x;
    double ydiff = a->loc_y - b->loc_y;
    return xdiff * xdiff + ydiff * ydiff;
}

int model_initialize(struct Model *m,
                     double width,
                     double height,
                     double blast_radius,
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

    if (blast_radius <= 0) {
        return 4;
    }
    m->blast_radius = blast_radius;
    m->blast_rasius_sq = blast_radius * blast_radius;

    mt19937_initialize(&m->gen, seed);

    if (target_max_acc <= 0) {
        return 5;
    }
    m->target.max_acc = target_max_acc;
    m->target.max_acc_sq = target_max_acc * target_max_acc;

    if (target_max_spd <= 0) {
        return 6;
    }
    m->target.max_spd = target_max_spd;
    m->target.max_spd_sq = target_max_spd * target_max_spd;

    if (missle_max_acc <= 0) {
        return 7;
    }
    m->missle.max_acc = missle_max_acc;
    m->missle.max_acc_sq = missle_max_acc * missle_max_acc;

    if (missle_max_spd <= 0) {
        return 8;
    }
    m->missle.max_spd = missle_max_spd;
    m->missle.max_spd_sq = missle_max_spd * missle_max_spd;
    
    m->stat = STAT_ONGOING;

    m->target.acc_x = 0;
    m->target.acc_y = 0;
    m->target.vel_x = 0;
    m->target.vel_y = 0;

    m->missle.acc_x = 0;
    m->missle.acc_y = 0;
    m->missle.vel_x = 0;
    m->missle.vel_y = 0;
    return 0;
}

int model_update(struct Model *m) {
    if (!m) {
        return 1;
    }
    entity_update_acc(&m->target, &m->gen);
    entity_update_vel(&m->target);
    entity_update_loc(&m->target);
    if (m->target.loc_x < 0 || m->target.loc_x >= m->width ||
        m->target.loc_y < 0 || m->target.loc_y >= m->height) {
        m->stat = STAT_FAILURE;
        return 0;
    }
    entity_chase_acc(&m->missle, &m->target);
    entity_update_vel(&m->missle);
    entity_update_loc(&m->missle);
    if (m->missle.loc_x < 0 || m->missle.loc_x >= m->width ||
        m->missle.loc_y < 0 || m->missle.loc_y >= m->height) {
        m->stat = STAT_FAILURE;
        return 0;
    }
    if (entity_dist_sq(&m->target, &m->missle) <= m->blast_rasius_sq) {
        m->stat = STAT_SUCCESS;
        return 0;
    }
}
