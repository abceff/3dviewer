#include "s21_3dviewer.h"

int main() {
    data DATA;
    FILE* f;
    f = fopen("example.obj", "r");
    if (f) {
        // считаем count_of_vertexes и count_of_facets
        DATA.count_of_vertexes = 0;
        DATA.count_of_facets = 0;
        count_vertexes_and_facets(f, &DATA);
        DATA.matrix_3d.rows = DATA.count_of_vertexes + 1;
        DATA.matrix_3d.cols = 3;
        // Выделям память для vertexes
        DATA.matrix_3d.matrix = malloc(DATA.matrix_3d.rows * sizeof(double*));
        for (int i = 0; i < DATA.matrix_3d.rows; i++) {
            DATA.matrix_3d.matrix[i] =
                malloc(DATA.matrix_3d.cols * sizeof(double));
        }

        // заносим в структуру vertexes и facets
        fseek(f, 0L, SEEK_SET);
        parse_vertexes(f, DATA);
        // parse_facets(f, DATA);
        for (int i = 1; i < DATA.matrix_3d.rows; i++) {
            for (int j = 0; j < DATA.matrix_3d.cols; j++) {
                printf("%lf ", DATA.matrix_3d.matrix[i][j]);
            }
            printf("\n");
        }

        // Очистка
        for (int i = 0; i < DATA.matrix_3d.rows + 1; i++)
            free(DATA.matrix_3d.matrix[i]);
        free(DATA.matrix_3d.matrix);
    }

    return 0;
}

void count_vertexes_and_facets(FILE* f, data* DATA) {
    char c;
    int line_start_flag = TRUE;
    int hash_flag = FALSE;
    while ((c = fgetc(f)) != EOF) {
        if (c == '#') hash_flag = TRUE;
        if (c != '\n' && hash_flag == TRUE) {
            continue;
        }
        if (c == 'v' && line_start_flag == TRUE && hash_flag == FALSE) {
            DATA->count_of_vertexes++;
        }
        if (c == 'f' && line_start_flag == TRUE && hash_flag == FALSE) {
            DATA->count_of_facets++;
        }
        if (c == '\n') {
            hash_flag = FALSE;
            line_start_flag = TRUE;
            continue;
        } else {
            line_start_flag = FALSE;
        }
    }
}

void parse_vertexes(FILE* f, data DATA) {
    char c;
    int line_start_flag = TRUE;
    int hash_flag = FALSE;
    for (int i = 1; i < DATA.matrix_3d.rows; i++) {
        c = fgetc(f);
        if (c == '#') hash_flag = TRUE;
        if (c != '\n' && hash_flag == TRUE) {
            continue;
        }
        if (c == 'v' && line_start_flag == TRUE && hash_flag == FALSE) {
            for (int j = 0; j < DATA.matrix_3d.cols; j++)
                fscanf(f, " %lf", &DATA.matrix_3d.matrix[i][j]);
        }
        if (c == '\n') {
            hash_flag = FALSE;
            line_start_flag = TRUE;
            continue;
        } else {
            line_start_flag = FALSE;
        }
    }
}

// void parse_facets(FILE* f, data DATA) {

// }