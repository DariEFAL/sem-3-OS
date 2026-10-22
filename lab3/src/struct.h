#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define THRESHOLD 3

typedef enum {
    READ_OK = 0,
    READ_ERR_OPEN,
    READ_ERR_MATRIX_SIZE,
    READ_ERR_NOMEM,
    READ_ERR_DATA
} ReadStatus;

typedef struct {
    double **matrix;
    size_t n;
    size_t tid; // номер потока (0..M-1)
    size_t M; // всего потоков
    double partial; // сюда поток сложит свою часть
} WorkerArg;

ReadStatus read_file(const char *path, size_t *n_out, double ***matrix_out);

int det_single_threaded(size_t n, double **matrix, double *res_out);
int make_minor(size_t n, double **matrix, size_t skip_row, size_t skip_col, double ***minor_out);

void *worker(void *arg);

void free_matrix(double **minor, size_t n);

void run_benchmark(void);