#include "runner/runner.h"

static const char *ext[5] = {
    "txt",
    "csv",
    "tsv",
    "json",
    "xml"
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
                   10,
                   1, 1500,
                   1, 1500,
                   1, 1500,
                   1, 1500,
                   i + 1, fout);
        fclose(fout);
    }
    puts("mode: bin");
    fout = fopen("out/data.bin", "wb");
    runner_run(10000, 24, 100000,
               1, 1500,
               1, 1500,
               1, 1500,
               1, 1500,
               10,
               1, 1500,
               1, 1500,
               1, 1500,
               1, 1500,
               LOG_BIN, fout);
    fclose(fout);
    return 0;
}
