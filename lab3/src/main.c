#include "struct.h"

int main(int argc, char *argv[]) {
    //считываем максимальное кол-во потоков, и путь к файлу с матрицей и размером
    // ./prog --threads n --matrix file_path
    size_t max_threads = 1;
    const char *path = NULL;

    for (int i = 1; i < argc; i++) {
        if (argc == 2 && strcmp(argv[1], "--benchSingle") == 0) {
            srand(42); 
            run_benchmark();
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

    
}