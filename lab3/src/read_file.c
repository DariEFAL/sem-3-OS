#include "struct.h"

ReadStatus read_file(const char *path, size_t *n_out, double ***matrix_out) {
    FILE *f = fopen(path, "r");
    if (!f) return READ_ERR_OPEN;

    size_t n;
    if (fscanf(f, "%zu", &n) != 1 || n <= 0) return READ_ERR_MATRIX_SIZE;

    double **matrix = malloc(n * sizeof(double *));
    if (!matrix) { 
        fclose(f); 
        return READ_ERR_NOMEM; 
    }
    for (size_t i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(double));
        if (!matrix[i]) {
            for (size_t k = 0; k < i; k++) free(matrix[k]);
            free(matrix);
            fclose(f);
            return READ_ERR_NOMEM;
        }
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (fscanf(f, "%lf", &matrix[i][j]) != 1) {
                for (size_t k = 0; k < n; k++) free(matrix[k]);
                free(matrix);
                fclose(f);
                return READ_ERR_DATA;
            }
        }
    }

    fclose(f);

    *n_out = n;
    *matrix_out = matrix;

    return READ_OK;
}