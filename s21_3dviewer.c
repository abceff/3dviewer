#include "s21_3dviewer.h"

int main() {
    data DATA;
    main_function("example.obj", &DATA);

    // Проверка
    for (int i = 0; i < DATA.count_of_vertexes * 3; i++) {
        printf("%lf ", DATA.matrix_1d[i]);
    }
    printf("\n");
    for (int i = 0; i < DATA.vertexes_connections_counter; i++) {
        printf("%d ", DATA.vertexes_connections[i]);
    }

    // Очистка
    free(DATA.matrix_1d);
    free(DATA.vertexes_connections);
}

int main_function(char* filename, data* DATA) {
    int return_value = OK;
    FILE* f;
    f = fopen(filename, "r");
    if (f) {
        // считаем count_of_vertexes и count_of_facets
        count_vertexes_and_facets(f, DATA);
        // Выделям память для matrix_1d
        DATA->matrix_1d =
            malloc(DATA->count_of_vertexes * 3 * sizeof(double*));
        // Выделяем память для poligons
        DATA->vertexes_connections =
            malloc((DATA->vertexes_connections_counter) * sizeof(int));

        // заносим в структуру vertexes и facets
        fseek(f, 0L, SEEK_SET);
        parse(f, DATA);
    } else {
        return_value = ERROR;
    }

    return return_value;
}

void count_vertexes_and_facets(FILE* f, data* DATA) {
    char c;
    int line_start_flag = TRUE;
    int hash_flag = FALSE;
    int f_flag = FALSE;
    DATA->count_of_vertexes = 0;
    DATA->vertexes_connections_counter = 0;
    int count_of_facets = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c == '#') hash_flag = TRUE;
        if (c != '\n' && hash_flag == TRUE) {
            continue;
        }
        if (c == 'v' && line_start_flag == TRUE && hash_flag == FALSE) {
            DATA->count_of_vertexes++;
        }
        if (c == 'f' && line_start_flag == TRUE && hash_flag == FALSE) {
            count_of_facets++;
            f_flag = TRUE;
        }
        if (c == ' ' && f_flag == TRUE) {
            DATA->vertexes_connections_counter++;
        }
        if (c == '\n') {
            hash_flag = FALSE;
            f_flag = FALSE;
            line_start_flag = TRUE;
            continue;
        } else {
            line_start_flag = FALSE;
        }
    }
    DATA->vertexes_connections_counter =
        2 * (DATA->vertexes_connections_counter - count_of_facets);
}

void parse(FILE* f, data* DATA) {
    char c;
    int line_start_flag = TRUE;
    int hash_flag = FALSE;
    int vertexes_counter = 0;
    int facets_counter = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c == '#') hash_flag = TRUE;
        if (c != '\n' && hash_flag == TRUE) {
            continue;
        }
        if (c == 'v' && line_start_flag == TRUE && hash_flag == FALSE) {
            fscanf(f, " %lf", &DATA->matrix_1d[vertexes_counter]);
            vertexes_counter++;
            fscanf(f, " %lf", &DATA->matrix_1d[vertexes_counter]);
            vertexes_counter++;
            fscanf(f, " %lf", &DATA->matrix_1d[vertexes_counter]);
            vertexes_counter++;
        }
        if (c == 'f' && line_start_flag == TRUE && hash_flag == FALSE) {
            parse_polygon(f, DATA, &facets_counter);
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

void parse_polygon(FILE* f, data* DATA, int* facets_counter) {
    int space_flag = FALSE;
    char* str = calloc(1024, sizeof(char));
    fscanf(f, "%[^\n]%*1[\n]", str);
    int str_len = (int)strlen(str);
    for (int i = 0; i < str_len; i++) {
        space_flag--;
        if (str[i] == ' ') space_flag = 2;

        if (str[i] - '0' >= 0 && str[i] - '0' <= 9 && space_flag == TRUE) {
            if (*facets_counter % 2 == 0) {
                sscanf(str + i, "%d",
                       &DATA->vertexes_connections[*facets_counter]);
                (*facets_counter)++;
            } else {
                sscanf(str + i, "%d",
                       &DATA->vertexes_connections[*facets_counter]);
                (*facets_counter)++;
                sscanf(str + i, "%d",
                       &DATA->vertexes_connections[*facets_counter]);
                (*facets_counter)++;
            }
            space_flag = FALSE;
        }
    }
    (*facets_counter)--;

    free(str);
}