#ifndef SRC_PARSE_H_
#define SRC_PARSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 256

typedef struct Matrix {
  float **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct Facets {           // полигоны
  int *vertexes;                  // array of apexes
  unsigned int numOfVerInFacets;  // count of apexes
  int numofone;
} polygon_t;

typedef struct data {
  int count_V;      // apex
  int count_F;      // polygons
  int count_EDGES;  // edges
  matrix_t *matrix_3d;
  polygon_t *polygons;
} data_struct;

matrix_t s21_create_matrix(int rows, int columns);
void s21_remove_matrix(matrix_t *A);

void counters_v(data_struct *data, FILE *fl);
void counters_f(data_struct *data, FILE *fl);
void matrixFill(data_struct *data, FILE *fl);
void polygonFill(data_struct *data, FILE *fl);

#endif  // SRC_PARSE_H_
