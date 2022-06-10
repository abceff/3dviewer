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
        // Выделям память для matrix_3d
        DATA.matrix_3d.matrix = malloc(DATA.matrix_3d.rows * sizeof(double*));
        for (int i = 1; i < DATA.matrix_3d.rows; i++) {
            DATA.matrix_3d.matrix[i] =
                malloc(DATA.matrix_3d.cols * sizeof(double));
        }
        // Выделяем память для poligons
        DATA.polygons = malloc((DATA.count_of_facets + 1) * sizeof(polygon_t));

        // заносим в структуру vertexes и facets
        fseek(f, 0L, SEEK_SET);
        parse(f, &DATA);

        // Проверка
        for (int i = 1; i < DATA.matrix_3d.rows; i++) {
            for (int j = 0; j < DATA.matrix_3d.cols; j++) {
                printf("%lf ", DATA.matrix_3d.matrix[i][j]);
            }
            printf("\n");
        }
        for (int i = 1; i < DATA.count_of_facets + 1; i++) {
            for (int j = 0; j < DATA.polygons[i].numbers_of_vertexes_in_facets; j++) {
                printf("%d ", DATA.polygons[i].vertexes[j]);
            }
            printf("\n");
        }

        // Очистка
        for (int i = 0; i < DATA.count_of_vertexes + 1; i++)
            free(DATA.matrix_3d.matrix[i]);
        free(DATA.matrix_3d.matrix);

        for (int i = 1; i < DATA.count_of_facets + 1; i++)
            free(DATA.polygons[i].vertexes);
        free(DATA.polygons);
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

void parse(FILE* f, data* DATA) {
    char c;
    int line_start_flag = TRUE;
    int hash_flag = FALSE;
    int vertexes_counter = 1;
    int facets_counter = 1;
    while ((c = fgetc(f)) != EOF) {
        if (c == '#') hash_flag = TRUE;
        if (c != '\n' && hash_flag == TRUE) {
            continue;
        }
        if (c == 'v' && line_start_flag == TRUE && hash_flag == FALSE) {
            for (int j = 0; j < DATA->matrix_3d.cols; j++)
                fscanf(f, " %lf", &DATA->matrix_3d.matrix[vertexes_counter][j]);
            vertexes_counter++;
        }
        if (c == 'f' && line_start_flag == TRUE && hash_flag == FALSE) {
            parse_polygon(f, DATA, facets_counter);
            facets_counter++;
            fseek(f, -1, SEEK_CUR);
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

void parse_polygon(FILE* f, data* DATA, int facets_counter) {
    int space_flag = FALSE;
    char* str = calloc(1024, sizeof(char));
    DATA->polygons[facets_counter].numbers_of_vertexes_in_facets = 0;
    fscanf(f, "%[^\n]%*1[\n]", str);
    int str_len = (int)strlen(str);
    for (int i = 0; i < str_len; i++) {
        space_flag--;
        if (str[i] == ' ') space_flag = 2;

        if (str[i] - '0' >= 0 && str[i] - '0' <= 9 && space_flag == TRUE) {
            DATA->polygons[facets_counter].numbers_of_vertexes_in_facets++;
            space_flag = FALSE;
        }
    }
    DATA->polygons[facets_counter].vertexes =
        malloc(DATA->polygons[facets_counter].numbers_of_vertexes_in_facets *
               sizeof(int));

    int vertexes_counter = 0;
    for (int i = 0; i < str_len; i++) {
        if (str[i] == ' ') {
            sscanf(str + i, "%d", &DATA->polygons[facets_counter].vertexes[vertexes_counter]);
            vertexes_counter++;
        }
    }
    
    free(str);
}