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
    int error; //код ошибки
} WorkerArg;


ReadStatus read_file(const char *path, size_t *n_out, double ***matrix_out);
void free_matrix(double **minor, size_t n);

//for single_threaded
int det_single_threaded(size_t n, double **matrix, double *res_out);
int make_minor(size_t n, double **matrix, size_t skip_row, size_t skip_col, double ***minor_out);

//for many threaded
void *worker(void *argc);
double det_parallel(size_t n, double **matrix, size_t M, int *err);

//benchmark
double now_sec(void);
double **gen_matrix(size_t n);

double measure_single_t(size_t n, double **matrix, double *det_out, int repeat);
void run_benchmark_single_t(void);

double measure_parallel(size_t n, double **matrix, size_t M, double *det_out, int repeat, int *err_out);
void run_benchmark_parallel(size_t M_max);