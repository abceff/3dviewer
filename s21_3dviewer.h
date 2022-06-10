// s21_3dviewer.h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { OK, ERROR };
enum { FALSE, TRUE };

typedef struct {
    double* matrix_1d;
    int count_of_vertexes;
    int* vertexes_connections;
    int vertexes_connections_counter;
} data;

void count_vertexes_and_facets(FILE* f, data* DATA);
void parse(FILE* f, data* DATA);
void parse_polygon(FILE* f, data* DATA, int* facets_counter);
int main_function(char* filename, data* DATA);