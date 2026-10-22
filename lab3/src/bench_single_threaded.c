#include "struct.h"
#include <time.h>

double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// Мерит det_single_threaded: REPEAT прогонов, возвращает МИНИМУМ времени
double measure_det(size_t n, double **matrix, double *det_out, int repeat) {
    double best = 1e18;
    for (int r = 0; r < repeat; r++) {
        double t0 = now_sec();
        det_single_threaded(n, matrix, det_out);
        double t1 = now_sec();
        double dt = t1 - t0;
        if (dt < best) best = dt;
    }
    return best;
}

double **gen_matrix(size_t n) {
    double **A = malloc(n * sizeof(*A));
    if (!A) return NULL;
    for (size_t i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(*A[i]));
        if (!A[i]) {
            for (size_t k = 0; k < i; k++) free(A[k]);
            free(A);
            return NULL;
        }
        for (size_t j = 0; j < n; j++)
            A[i][j] = rand() % 19 - 9;   // -9..9
    }
    return A;
}

void run_benchmark(void) {
    printf("%4s %14s %16s\n", "n", "time_s", "det");
    printf("--------------------------------------\n");
    for (size_t n = 3; n <= 13; n++) {
        double **A = gen_matrix(n);
        if (!A) { printf("no memory for n=%zu\n", n); return; }

        double det = 0;
        int repeat = (n <= 5) ? 1000 : (n <= 7 ? 10 : 1);
        double t = measure_det(n, A, &det, repeat);

        printf("%4zu %14.6f %16.6f\n", n, t, det);
        fflush(stdout); // чтобы строки печатались сразу, а не в конце
        free_matrix(A, n);
    }
}