#include "engine.h"

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
                      double missle_max_spd) {
    if (!e) {
        return 1;
    }
    e->n_thread = n_thread;
    e->n_iter = n_iter;
    int ret_val = model_initialize(&e->model,
                                   dist,
                                   fuel,
                                   rate,
                                   blast_radius,
                                   seed,
                                   target_max_acc,
                                   target_max_spd,
                                   missle_max_acc,
                                   missle_max_spd);
    return ret_val;
}

int engine_step(struct Engine *e, char verbose, FILE *fout) {
    if (!e || !fout) {
        return 1;
    }
    int ret_val = model_update(&e->model);
    if (ret_val) {
        return ret_val;
    }
    if (verbose) {
        model_log(&e->model, fout);
    }
    return 0;
}

int engine_one_iter(struct Engine *e, char *success, char verbose, FILE *fout) {
    int ret_val = 0;
    for (int i = 0; !e->model.stat; ++i) {
        if (verbose) {
            fprintf(fout, "time: %d\n", i);
        }
        ret_val = engine_step(e, verbose, fout);
        if (ret_val) {
            return ret_val;
        }
    }
    *success = (e->model.stat == 1);
    return 0;
}
