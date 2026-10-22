#include "struct.h"
#include <time.h>
#include <math.h>

double measure_parallel(size_t n, double **matrix, size_t M, double *det_out,
                        int repeat, int *err_out) {
    double best = 1e18;
    *err_out = 0;
    for (int r = 0; r < repeat; r++) {
        int err = 0;
        double t0 = now_sec();
        *det_out = det_parallel(n, matrix, M, &err);
        double t1 = now_sec();
        if (err != 0) { *err_out = err; return best; }
        double dt = t1 - t0;
        if (dt < best) best = dt;
    }
    return best;
}

void run_benchmark_parallel(size_t M_max) {
    printf("%4s %12s %16s %16s %22s %10s %12s\n",
           "n", "max_threads", "time_single_t", "time_parallel", "det", "speedup", "efficiency");
    printf("----------------------------------------------------------------------------------------------\n");

    for (size_t n = 3; n <= 11; n++) {
        double **A = gen_matrix(n);
        if (!A) {
            printf("no memory for n=%zu\n", n);
            return;
        }

        double det_seq = 0;
        int rep = (n <= 6) ? 1000 : (n <= 8 ? 100 : 10);
        double T1 = measure_single_t(n, A, &det_seq, rep);

        for (size_t M = 1; M <= M_max; M *= 2) {
            int err = 0;
            double det_par = 0;
            double T_M = measure_parallel(n, A, M, &det_par, rep, &err);

            if (err) {
                printf("%4zu %12zu   ERROR err=%d\n", n, M, err);
                continue;
            }

            double speedup = (T_M > 0.0) ? T1 / T_M : 0.0;
            double efficiency = (M > 0) ? speedup / (double)M : 0.0;

            printf("%4zu %12zu %16.6f %16.6f %22.6f %10.2f %12.3f\n",
                   n, M, T1, T_M, det_par, speedup, efficiency);

            double tol = 1e-6 * fabs(det_seq) + 1e-9;
            if (fabs(det_par - det_seq) > tol) {
                printf("   !! warning: n=%zu M=%zu det mismatch: par=%.6f seq=%.6f\n",
                       n, M, det_par, det_seq);
            }
        }

        free_matrix(A, n);
        printf("----------------------------------------------------------------------------------------------\n");
    }
}