// s21_3dviewer.h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { OK, ERROR };
enum { FALSE, TRUE };

typedef struct {
    double** matrix;
    int rows;
    int cols;
} matrix_t;

typedef struct {
    int* vertexes;
    int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct {
    int count_of_vertexes;
    int count_of_facets;
    matrix_t matrix_3d;
    polygon_t* polygons;
} data;

void count_vertexes_and_facets(FILE* f, data* DATA);
void parse(FILE* f, data* DATA);
void parse_polygon(FILE* f, data* DATA, int facets_counter);