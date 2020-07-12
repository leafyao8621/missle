#include "runner/runner.h"

static const char mode[5] = {
    LOG_CSV,
    LOG_TSV,
    LOG_JSON,
    LOG_XML,
    LOG_TEXT
};

static const char *ext[5] = {
    "csv",
    "tsv",
    "json",
    "xml",
    "txt"
};

static char buf[100];

int main(void) {
    FILE *fout;
    for (int i = 0; i < 5; ++i) {
        printf("mode: %s\n", ext[i]);
        snprintf(buf, 99, "out/data.%s", ext[i]);
        fout = fopen(buf, "w");
        runner_run(10000, 24, 100000,
                   1, 1500,
                   1, 1500,
                   1, 1500,
                   1, 1500,
                   100,
                   1, 1500,
                   1, 1500,
                   1, 1500,
                   1, 1500,
                   1,
                   mode[i], fout);
        fclose(fout);
    }
    puts("mode: bin");
    fout = fopen("out/data.bin", "wb");
    runner_run(10000, 24, 100000,
               1, 1500,
               1, 1500,
               1, 1500,
               1, 1500,
               100,
               1, 1500,
               1, 1500,
               1, 1500,
               1, 1500,
               1,
               LOG_BIN, fout);
    fclose(fout);
    return 0;
}
