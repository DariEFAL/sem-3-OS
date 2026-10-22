#include "struct.h"

int make_minor(size_t n, double **matrix, size_t skip_row, size_t skip_col, double ***minor_out) {
    if (!matrix) return -2;
    if (skip_row >= n || skip_col >= n) return -3;

    double **minor = malloc((n - 1) * sizeof(double *));
    if (!minor) {
        return -1; 
    }
    for (size_t i = 0; i < n - 1; i++) {
        minor[i] = malloc((n - 1) * sizeof(double));
        if (!minor[i]) {
            for (size_t k = 0; k < i; k++) free(minor[k]);
            free(minor);
            return -1;
        }
    }

    size_t mi = 0;
    for (size_t i = 0; i < n; i++) {
        if (i == skip_row) continue;
        size_t mj = 0;
        for (size_t j = 0; j < n; j++) {
            if (j == skip_col) continue;
            minor[mi][mj] = matrix[i][j];
            mj++;
        }
        mi++;
    }

    *minor_out = minor;
    return 0;
}

int det_single_threaded(size_t n, double **matrix, double *res_out) {
    if (!matrix) return -2;
    if (n <= 0) return -3;
    if (n == 1) {
        *res_out = matrix[0][0];
        return 0;
    }
    if (n == 2) {
        *res_out = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        return 0;
    }


    double res = 0;
    for (size_t j = 0; j < n; j++) {
        double **minor = NULL;
        int rc = make_minor(n, matrix, 0, j, &minor);
        if (rc != 0) return rc;

        double sub;
        rc = det_single_threaded(n - 1, minor, &sub);
        if (rc != 0) {
            free_matrix(minor, n - 1);
            return rc;
        }

        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        res += sign * matrix[0][j] * sub;
        free_matrix(minor, n - 1);
    }
    *res_out = res;
    return 0;
    
}