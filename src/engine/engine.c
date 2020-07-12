#include <stdlib.h>
#include <pthread.h>
#include "engine.h"
#include "../util/mt19937.h"

struct SubEngine {
    unsigned id;
    struct Engine engine;
    char verbose;
    FILE *fout;
    double success_cnt;
};

static void *sub_runner(void *sub_engine) {
    struct SubEngine *se = (struct SubEngine*)sub_engine;
    unsigned n_iter = se->engine.n_iter / se->engine.n_thread;
    if (se->id == se->engine.n_thread - 1) {
        n_iter += se->engine.n_iter % se->engine.n_thread;
    }

    char success = 0;
    double success_cnt = 0;
    for (unsigned i = 0, ii = se->id * se->engine.n_iter / se->engine.n_thread;
         i < n_iter; ++i, ++ii) {
        engine_one_iter(&se->engine, &success, 0, 0);
        success_cnt += success;
        if (se->verbose) {
            fprintf(se->fout, "iter: %u success: %hhd\n", ii, success);
        }
    }
    se->success_cnt = success_cnt;
    return 0;
}

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
    e->seed = seed;
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
    if (!e || (verbose && !fout)) {
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
    if (!e || !success || (verbose && !fout)) {
        return 1;
    }

    e->model.stat = STAT_ONGOING;
    e->model.fuel = e->model.max_fuel;

    e->model.target.acc_x = 0;
    e->model.target.acc_y = 0;
    e->model.target.vel_x = 0;
    e->model.target.vel_y = 0;
    e->model.target.loc_x = 0;
    e->model.target.loc_y = 0;

    e->model.missle.acc_x = 0;
    e->model.missle.acc_y = 0;
    e->model.missle.vel_x = 0;
    e->model.missle.vel_y = 0;
    e->model.missle.loc_x = e->model.dist;
    e->model.missle.loc_y = 0;

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

int engine_run(struct Engine *e, double *prob, char verbose, FILE *fout) {
    int ret_val = 0;
    if (!e || !prob || (verbose && !fout)) {
        return 1;
    }

    double n_thread = e->n_thread;
    struct SubEngine *sub_engines =
        malloc(sizeof(struct SubEngine) * e->n_thread);
    struct SubEngine *sub_engines_iter = sub_engines;
    pthread_t *pool =
        malloc(sizeof(pthread_t) * e->n_thread);
    pthread_t *pool_iter = pool;
    for (unsigned i = 0, seed = e->seed;
         i < n_thread;
         ++i, ++seed, ++sub_engines_iter, ++pool_iter) {
        sub_engines_iter->id = i;
        sub_engines_iter->engine = *e;
        mt19937_initialize(&sub_engines_iter->engine.model.gen, seed);
        sub_engines_iter->verbose = verbose;
        sub_engines_iter->fout = fout;
        pthread_create(pool_iter, 0, sub_runner, sub_engines_iter);
    }

    sub_engines_iter = sub_engines;
    pool_iter = pool;
    double success_cnt = 0;
    for (unsigned i = 0; i < n_thread; ++i, ++sub_engines_iter, ++pool_iter) {
        pthread_join(*pool_iter, 0);
        success_cnt += sub_engines_iter->success_cnt;
    }
    free(sub_engines);
    free(pool);
    *prob = success_cnt / e->n_iter;
    return 0;
}
