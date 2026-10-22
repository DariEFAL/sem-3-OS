#include "struct.h"
#include <math.h>

#define SEED 42

int main(int argc, char *argv[]) {
    //считываем максимальное кол-во потоков, и путь к файлу с матрицей и размером
    // ./prog --threads n --matrix file_path
    size_t max_threads = 1;
    const char *path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--benchSingleT") == 0) {
            srand(SEED); 
            run_benchmark_single_t();
            return 0;
        }
        else if (strcmp(argv[i], "--benchParallel") == 0) {
            srand(SEED); 
            run_benchmark_parallel(12);
            return 0;
        }
        else if (strcmp(argv[i], "--threads") == 0) {
            if (i + 1 >= argc) { 
                fprintf(stderr, "--threads: нет значения\n"); 
                return 1; 
            }
            const char *s = argv[++i];
            char *end = NULL;
            errno = 0;
            long v = strtol(s, &end, 10);
            if (errno || end == s || *end != '\0' || v < 1 || v > 1024) {
                fprintf(stderr, "--threads: ожидается целое 1..1024, получено '%s'\n", s);
                return 1;
            }
            max_threads = (size_t)v;
        }
        else if (strcmp(argv[i], "--matrix") == 0) {
            if (i + 1 >= argc) { 
                fprintf(stderr, "--matrix: нет значения\n"); 
                return 1; 
            }
            path = argv[++i];
        }
        else {
            fprintf(stderr, "Неизвестный аргумент: %s\n", argv[i]);
            return 1;
        }
    }

    if (path == NULL) {
        fprintf(stderr, "Не указан --input\n");
        return 1;
    }

    size_t n;
    double **matrix = NULL;
    ReadStatus rs = read_file(path, &n, &matrix);
    if (rs != READ_OK) {
        fprintf(stderr, "read file failed: code %d\n", rs);
        return 1;
    }

    int err = 0;
    double det = det_parallel(n, matrix, max_threads, &err);
    if (err != 0) {
        fprintf(stderr, "det_parallel failed: %d\n", err);
        free_matrix(matrix, n);
        return 1;
    }

    printf("n = %zu, M = %zu\n", n, max_threads);
    printf("det = %.10f\n", det);

    double det_seq = 0;
    int rc_seq = det_single_threaded(n, matrix, &det_seq);
    if (rc_seq == 0) {
        printf("det_single = %.10f, diff = %.2e\n", det_seq, fabs(det - det_seq));
    }

    free_matrix(matrix, n);
    return 0;
}