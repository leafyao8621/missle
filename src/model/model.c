#include <math.h>
#include "../util/generator.h"
#include "model.h"

static const char *stat_lookup[3] = {
    "STAT_ONGOING",
    "STAT_SUCCESS",
    "STAT_FAILURE"
};

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
        e->vel_x *= rat;
        e->vel_y *= rat;
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
                     double dist,
                     double fuel,
                     double rate,
                     double blast_radius,
                     unsigned seed,
                     double target_max_acc,
                     double target_max_spd,
                     double missle_max_acc,
                     double missle_max_spd) {
    if (!m) {
        return 1;
    }

    if (dist <= 0) {
        return 2;
    }
    m->dist = dist;
    m->missle.loc_x = dist;

    if (blast_radius <= 0) {
        return 3;
    }
    m->blast_radius = blast_radius;
    m->blast_rasius_sq = blast_radius * blast_radius;

    if (fuel <= 0) {
        return 4;
    }
    m->fuel = fuel;

    if (rate <= 0) {
        return 5;
    }
    m->rate = rate;

    mt19937_initialize(&m->gen, seed);

    if (target_max_acc <= 0) {
        return 6;
    }
    m->target.max_acc = target_max_acc;
    m->target.max_acc_sq = target_max_acc * target_max_acc;

    if (target_max_spd <= 0) {
        return 7;
    }
    m->target.max_spd = target_max_spd;
    m->target.max_spd_sq = target_max_spd * target_max_spd;

    if (missle_max_acc <= 0) {
        return 8;
    }
    m->missle.max_acc = missle_max_acc;
    m->missle.max_acc_sq = missle_max_acc * missle_max_acc;

    if (missle_max_spd <= 0) {
        return 9;
    }
    m->missle.max_spd = missle_max_spd;
    m->missle.max_spd_sq = missle_max_spd * missle_max_spd;
    
    m->stat = STAT_ONGOING;

    m->target.acc_x = 0;
    m->target.acc_y = 0;
    m->target.vel_x = 0;
    m->target.vel_y = 0;
    m->target.loc_x = 0;
    m->target.loc_x = 0;

    m->missle.acc_x = 0;
    m->missle.acc_y = 0;
    m->missle.vel_x = 0;
    m->missle.vel_y = 0;
    m->missle.loc_y = 0;

    return 0;
}

int model_update(struct Model *m) {
    if (!m) {
        return 1;
    }

    entity_update_acc(&m->target, &m->gen);
    entity_update_vel(&m->target);
    entity_update_loc(&m->target);

    entity_chase_acc(&m->missle, &m->target);
    entity_update_vel(&m->missle);
    entity_update_loc(&m->missle);

    if (entity_dist_sq(&m->target, &m->missle) <= m->blast_rasius_sq) {
        m->stat = STAT_SUCCESS;
        return 0;
    }

    m->fuel -= m->rate *
               (m->missle.acc_x * m->missle.acc_x +
                m->missle.acc_y * m->missle.acc_y);
    
    if (m->fuel <= 0) {
        m->stat = STAT_FAILURE;
        return 0;
    }

    return 0;
}

int model_log(struct Model *m, FILE *fout) {
    fprintf(fout, "stat: %s\n", stat_lookup[m->stat]);
    fprintf(fout, "%s\n", "missle:");
    fprintf(fout, "fuel: %lf\n", m->fuel);
    fprintf(fout, "acc: %lf %lf\n", m->missle.acc_x, m->missle.acc_y);
    fprintf(fout, "vel: %lf %lf\n", m->missle.vel_x, m->missle.vel_y);
    fprintf(fout, "loc: %lf %lf\n", m->missle.loc_x, m->missle.loc_y);
    fprintf(fout, "%s\n", "target:");
    fprintf(fout, "acc: %lf %lf\n", m->target.acc_x, m->target.acc_y);
    fprintf(fout, "vel: %lf %lf\n", m->target.vel_x, m->target.vel_y);
    fprintf(fout, "loc: %lf %lf\n", m->target.loc_x, m->target.loc_y);
}
