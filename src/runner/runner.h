#ifndef _RUNNER_H_
#define _RUNNER_H_

#include <stdio.h>

#define LOG_NONE 0
#define LOG_TEXT 1
#define LOG_CSV  2
#define LOG_TSV  3
#define LOG_JSON 4
#define LOG_XML  5
#define LOG_BIN  6

int runner_run(unsigned n_param,
               unsigned n_thread,
               unsigned n_iter,
               double dist_low, double dist_high,
               double fuel_low, double fuel_high,
               double rate_low, double rate_high,
               double blast_radius_low, double blast_radius_high,
               unsigned seed,
               double target_max_acc_low, double target_max_acc_high,
               double target_max_spd_low, double target_max_spd_high,
               double missle_max_acc_low, double missle_max_acc_high,
               double missle_max_spd_low, double missle_max_spd_high,
               char log_mode, FILE *fout);

#endif
