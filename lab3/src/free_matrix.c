#include "struct.h"

void free_matrix(double **minor, size_t n) {
    if (!minor) return;
    for (size_t i = 0; i < n; i++) {
        free(minor[i]);
    }
    free(minor);
}