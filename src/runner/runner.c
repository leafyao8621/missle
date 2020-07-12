#include "runner.h"
#include "../engine/engine.h"
#include "../util/generator.h"

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
               char stat,
               char log_mode, FILE *fout) {
    if (log_mode && !fout) {
        return 1;
    }
    struct MT19937 gen;
    mt19937_initialize(&gen, seed);
    double dist, fuel, rate, blast_radius, target_max_acc, target_max_spd,
           missle_max_acc, missle_max_spd;
    struct Engine engine;
    double prob;
    switch (log_mode) {
    case LOG_CSV:
        fprintf(fout, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                "dist",
                "fuel",
                "rate",
                "blast_radius",
                "target_max_acc",
                "target_max_spd",
                "missle_max_acc",
                "missle_max_spd",
                "prob");
        break;
    case LOG_TSV:
        fprintf(fout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
                "dist",
                "fuel",
                "rate",
                "blast_radius",
                "target_max_acc",
                "target_max_spd",
                "missle_max_acc",
                "missle_max_spd",
                "prob");
        break;
    case LOG_JSON:
        fprintf(fout, "%c\n", '[');
        break;
    case LOG_XML:
        fprintf(fout, "%s\n", "<data>");
        break;
    case LOG_BIN:
        fwrite(&n_param, sizeof(unsigned), 1, fout);
        break;
    }
    for (unsigned i = 0; i < n_param; ++i) {
        generator_unif(&gen, dist_low, dist_high, &dist);
        generator_unif(&gen, fuel_low, fuel_high, &fuel);
        generator_unif(&gen, rate_low, rate_high, &rate);
        generator_unif(&gen, blast_radius_low, blast_radius_high,
                       &blast_radius);
        generator_unif(&gen, target_max_acc_low, target_max_acc_high,
                       &target_max_acc);
        generator_unif(&gen, target_max_spd_low, target_max_spd_high,
                       &target_max_spd);
        generator_unif(&gen, missle_max_acc_low, missle_max_acc_high,
                       &missle_max_acc);
        generator_unif(&gen, missle_max_spd_low, missle_max_spd_high,
                       &missle_max_spd);
        engine_initialize(&engine,
                          n_thread,
                          n_iter,
                          dist,
                          fuel,
                          rate,
                          blast_radius,
                          seed,
                          target_max_acc,
                          target_max_spd,
                          missle_max_acc,
                          missle_max_spd);
        engine_run(&engine, &prob, 0, 0);
        if (stat) {
            printf("setting: %u\n", i);
        }
        if (log_mode == LOG_TEXT) {
            fprintf(fout, "setting: %u\n", i);
        }
        switch (log_mode) {
        case LOG_TEXT:
            fprintf(fout, "dist: %lf\n", dist);
            fprintf(fout, "fuel: %lf\n", fuel);
            fprintf(fout, "rate: %lf\n", rate);
            fprintf(fout, "blast_radius: %lf\n", blast_radius);
            fprintf(fout, "target_max_acc: %lf\n", target_max_acc);
            fprintf(fout, "target_max_spd: %lf\n", target_max_spd);
            fprintf(fout, "missle_max_acc: %lf\n", missle_max_acc);
            fprintf(fout, "missle_max_spd: %lf\n", missle_max_spd);
            fprintf(fout, "prob: %lf\n", prob);
            break;
        case LOG_CSV:
            fprintf(fout, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                    dist,
                    fuel,
                    rate,
                    blast_radius,
                    target_max_acc,
                    target_max_spd,
                    missle_max_acc,
                    missle_max_spd,
                    prob);
            break;
        case LOG_TSV:
            fprintf(fout, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
                    dist,
                    fuel,
                    rate,
                    blast_radius,
                    target_max_acc,
                    target_max_spd,
                    missle_max_acc,
                    missle_max_spd,
                    prob);
            break;
        case LOG_JSON:
            fprintf(fout, "    %c\n", '{');
            fprintf(fout, "        \"dist\": %lf,\n", dist);
            fprintf(fout, "        \"fuel\": %lf,\n", fuel);
            fprintf(fout, "        \"rate\": %lf,\n", rate);
            fprintf(fout, "        \"blast_radius\": %lf,\n", blast_radius);
            fprintf(fout, "        \"target_max_acc\": %lf,\n", target_max_acc);
            fprintf(fout, "        \"target_max_spd\": %lf,\n", target_max_spd);
            fprintf(fout, "        \"missle_max_acc\": %lf,\n", missle_max_acc);
            fprintf(fout, "        \"missle_max_spd\": %lf,\n", missle_max_spd);
            fprintf(fout, "        \"prob\": %lf\n", prob);
            fprintf(fout, "    %c", '}');
            if (i < n_param - 1) {
                fputc(',', fout);
            }
            fputc(10, fout);
            break;
        case LOG_XML:
            fprintf(fout, "    %s\n", "<entry>");
            fprintf(fout, "        <dist>%lf</dist>\n", dist);
            fprintf(fout, "        <fuel>%lf</fuel>\n", fuel);
            fprintf(fout, "        <rate>%lf</rate>\n", rate);
            fprintf(fout, "        <blast_radius>%lf</blast_radius>\n",
                    blast_radius);
            fprintf(fout, "        <target_max_acc>%lf</target_max_acc>\n",
                    target_max_acc);
            fprintf(fout, "        <target_max_spd>%lf</target_max_spd>\n",
                    target_max_spd);
            fprintf(fout, "        <missle_max_acc>%lf</missle_max_acc>\n",
                    missle_max_acc);
            fprintf(fout, "        <missle_max_spd>%lf</missle_max_spd>\n",
                    missle_max_spd);
            fprintf(fout, "        <prob>%lf</prob>\n", prob);
            fprintf(fout, "    %s\n", "</entry>");
            break;
        case LOG_BIN:
            fwrite(&dist, sizeof(double), 1, fout);
            fwrite(&fuel, sizeof(double), 1, fout);
            fwrite(&rate, sizeof(double), 1, fout);
            fwrite(&blast_radius, sizeof(double), 1, fout);
            fwrite(&target_max_acc, sizeof(double), 1, fout);
            fwrite(&target_max_spd, sizeof(double), 1, fout);
            fwrite(&missle_max_acc, sizeof(double), 1, fout);
            fwrite(&missle_max_spd, sizeof(double), 1, fout);
            fwrite(&prob, sizeof(double), 1, fout);
            break;
        }
    }
    switch (log_mode) {
    case LOG_JSON:
        fprintf(fout, "%c\n", ']');
        break;
    case LOG_XML:
        fprintf(fout, "%s\n", "</data>");
        break;
    }
    return 0;
}
