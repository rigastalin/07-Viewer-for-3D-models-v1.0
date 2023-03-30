#include "parse.h"

#include <stdio.h>

matrix_t s21_create_matrix(int rows, int columns) {
  matrix_t new_matrix;
  new_matrix.rows = rows;
  new_matrix.columns = columns;
  new_matrix.matrix = (float **)calloc(new_matrix.rows, sizeof(float *));
  for (int i = 0; i < new_matrix.rows; i++) {
    new_matrix.matrix[i] = (float *)calloc(new_matrix.columns, sizeof(float));
  }
  return new_matrix;
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
    A->matrix[i] = NULL;
  }
  free(A->matrix);
  A->matrix = NULL;
}

void counters_v(data_struct *data, FILE *fl) {
  size_t length = NMAX;
  char *string = (char *)calloc(NMAX, sizeof(char));
  if (fl != NULL) {
    int i = 0;
    fseek(fl, 0, SEEK_SET);
    while (!feof(fl)) {
      getline(&string, &length, fl);
      // printf("%s\n", string);
      if (strncmp(string, "v ", 2) == 0) {
        data->count_V = data->count_V + 1;
        // printf("%d\n", data->count_V);
      }
      if (strncmp(string, "f ", 2) == 0) {
        data->count_F = data->count_F + 1;
      }
      i++;
    }
  } else {
    fprintf(stderr, "No such file, mafaka");
  }
  data->polygons = (polygon_t *)calloc(data->count_F, sizeof(polygon_t));
  data->matrix_3d = (matrix_t *)calloc(data->count_V, sizeof(matrix_t));
  for (int i = 0; i < data->count_V; i++) {
    data->matrix_3d[i] = s21_create_matrix(3, 1);
  }
  // printf("%d", data->count_V);
  free(string);
}

void counters_f(data_struct *data, FILE *fl) {
  char *string = (char *)calloc(NMAX, sizeof(char));
  size_t length = NMAX;
  if (fl != NULL) {
    int count = 0;
    fseek(fl, 0, SEEK_SET);
    size_t i = 0;
    while (!feof(fl)) {
      getline(&string, &length, fl);
      if (strncmp(string, "f ", 2) == 0) {
        char *token = strtok(string, "f ");
        while (token != NULL) {
          int digit = atoi(token);
          if (digit > 0) count = count + 1;
          token = strtok(NULL, "f ");
          // printf("%d ", digit);
        }
        data->polygons[i].numofone = count * 2;
        data->count_EDGES += count;
        count = 0;
        data->polygons[i].numOfVerInFacets = data->count_EDGES;
        data->polygons[i].vertexes =
            (int *)calloc(data->polygons[i].numofone, sizeof(int));
        i++;
      }
    }
  } else {
    fprintf(stderr, "No such file, mafaka");
  }
  free(string);
}

void matrixFill(data_struct *data, FILE *fl) {
  char *string = (char *)calloc(NMAX, sizeof(char));
  size_t length = NMAX;
  if (fl != NULL) {
    fseek(fl, 0, SEEK_SET);
    size_t i = 0;
    while (!feof(fl)) {
      getline(&string, &length, fl);
      if (strncmp(string, "v ", 2) == 0) {
        sscanf(string, "v %f %f %f", &data->matrix_3d[i].matrix[0][0],
               &data->matrix_3d[i].matrix[1][0],
               &data->matrix_3d[i].matrix[2][0]);
        // printf("%f %f %f\n", data->matrix_3d[i].matrix[0][0],
        //  data->matrix_3d[i].matrix[1][0],
        //  data->matrix_3d[i].matrix[2][0]);
        i++;
      }
    }
  } else {
    fprintf(stderr, "No such file, mafaka");
  }
  free(string);
}

void polygonFill(data_struct *data, FILE *fl) {
  char *string = (char *)calloc(NMAX, sizeof(char));
  size_t length = NMAX;
  if (fl != NULL) {
    fseek(fl, 0, SEEK_SET);
    size_t i = 0;
    while (!feof(fl)) {
      getline(&string, &length, fl);
      if (strncmp(string, "f ", 2) == 0) {
        int lengtString = strlen(string);
        char *token = strtok(string, "f ");
        int j = 0;
        while (token != NULL) {
          if (j == data->polygons[i].numofone - 1) break;
          int digit = atoi(token) - 1;
          data->polygons[i].vertexes[j] = digit;
          token = strtok(NULL, "f ");
          // printf("%d ", lastDigit);
          lengtString++;
          // printf("%d ", data->polygons[i].vertexes[j]);
          j++;
          if (j < data->polygons[i].numofone - 1 && j > 1) {
            data->polygons[i].vertexes[j] = data->polygons[i].vertexes[j - 1];
            // printf("%d ", data->polygons[i].vertexes[j]);
            j++;
          }
        }
        data->polygons[i].vertexes[j] = data->polygons[i].vertexes[0];
        // printf("%d ", data->polygons[i].vertexes[j]);
        j++;
        // printf("\n");
        i++;
      }
    }
    // printf("\n %d \n", lengtString);
  }
  free(string);
}

// int main() {
//   data_struct data;
//   FILE *file =
//       fopen("/Users/torellC8_3DViewer_v1.0-1/src/objects/cube_test2.obj",
//       "r");
//   counters_v(&data, file);
//   counters_f(&data, file);
//   matrixFill(&data, file);
//   polygonFill(&data, file);

//   return 0;
// }
