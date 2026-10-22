#include "struct.h"

void *worker(void *argc) {
    WorkerArg *arg = (WorkerArg*)argc;
    arg->partial = 0;

    for (size_t j = arg->tid; j < arg->n; j += arg->M) {
        double **minor = NULL;
        int rc = make_minor(arg->n, arg->matrix, 0, j, &minor);
        if (rc != 0) { 
            arg->error = rc; 
            return NULL;
        }

        double sub;
        rc = det_single_threaded(arg->n - 1, minor, &sub);
        if (rc != 0) {
            free_matrix(minor, arg->n - 1);
            arg->error = rc;
            return NULL;
        }

        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        arg->partial += sign * arg->matrix[0][j] * sub;
        free_matrix(minor, arg->n - 1);
    }

    return NULL;
}

double det_parallel(size_t n, double **matrix, size_t M, int *err) {
    *err = 0;
    if (M > n) M = n;

    WorkerArg *args = malloc(M * sizeof(WorkerArg));
    pthread_t *tids = malloc(M * sizeof(pthread_t));

    for (size_t i = 0; i < M; i++) {
        args[i].matrix = matrix;
        args[i].n = n;
        args[i].tid = i;
        args[i].M = M;
        args[i].partial = 0;
        args[i].error = 0;

        int rc = pthread_create(&tids[i], NULL, worker, &args[i]);
        if (rc != 0) {
            *err = rc;
            for (size_t k = 0; k < i; k++) pthread_join(tids[k], NULL);
            free(args);
            free(tids);
            return 0;
        }
    }

    for (size_t i = 0; i < M; i++) {
        pthread_join(tids[i], NULL);
        if (args[i].error != 0) {
            *err = args[i].error;
            free(args);
            free(tids);
            return 0;
        }
    }

    double det = 0;
    for (size_t i = 0; i < M; i++)
        det += args[i].partial;

    free(args);
    free(tids);

    return det;
}