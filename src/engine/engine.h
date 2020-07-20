#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "../model/model.h"

struct Engine {
    unsigned n_thread;
    unsigned n_iter;
    unsigned seed;
    struct Model model;
};

int engine_initialize(struct Engine *e,
                      unsigned n_thread,
                      unsigned n_iter,
                      double dist,
                      double fuel,
                      double rate,
                      double blast_radius,
                      unsigned seed,
                      double target_max_acc,
                      double target_max_spd,
                      double missle_max_acc,
                      double missle_max_spd);
int engine_step(struct Engine *e, char verbose, FILE *fout);
int engine_one_iter(struct Engine *e, char *success, char verbose, FILE *fout);
int engine_run(struct Engine *e, double *prob, char verbose, FILE *fout);

#endif
