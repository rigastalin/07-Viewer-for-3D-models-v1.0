#include "afins.h"

matrix_t s21_mult_number(matrix_t *A, double number) {
  matrix_t res;
  res = s21_create_matrix(A->rows, A->columns);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      res.matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return res;
}

void scale(data_struct *data, double val) {
  for (int i = 0; i < data->count_V; i++) {
    matrix_t temp = data->matrix_3d[i];
    data->matrix_3d[i] = s21_mult_number(&temp, val);
    s21_remove_matrix(&temp);
  }
}

void moveX(data_struct *data, double val) {
  for (int i = 0; i < data->count_V; i++) {
    data->matrix_3d[i].matrix[0][0] += val;
  }
}

void moveY(data_struct *data, double val) {
  for (int i = 0; i < data->count_V; i++) {
    data->matrix_3d[i].matrix[1][0] += val;
  }
}

void moveZ(data_struct *data, double val) {
  for (int i = 0; i < data->count_V; i++) {
    data->matrix_3d[i].matrix[2][0] += val;
  }
}

void rotationX(data_struct *data, double val) {
  for (int i = 0; i < data->count_V; i++) {
    double tempY = data->matrix_3d[i].matrix[1][0];
    double tempZ = data->matrix_3d[i].matrix[2][0];
    data->matrix_3d[i].matrix[1][0] = cos(val) * tempY - sin(val) * tempZ;
    data->matrix_3d[i].matrix[2][0] = sin(val) * tempY + cos(val) * tempZ;
  }
}

void rotationY(data_struct *data, double val) {
  for (int i = 0; i < data->count_V; i++) {
    double tempX = data->matrix_3d[i].matrix[0][0];
    double tempZ = data->matrix_3d[i].matrix[2][0];
    data->matrix_3d[i].matrix[0][0] = cos(val) * tempX + sin(val) * tempZ;
    data->matrix_3d[i].matrix[2][0] = (-1) * sin(val) * tempX + cos(val) * tempZ;
  }
}

void rotationZ(data_struct *data, double val) {
  for (int i = 0; i < data->count_V; i++) {
    double tempX = data->matrix_3d[i].matrix[0][0];
    double tempY = data->matrix_3d[i].matrix[1][0];
    data->matrix_3d[i].matrix[0][0] = cos(val) * tempX - sin(val) * tempY;
    data->matrix_3d[i].matrix[1][0] = sin(val) * tempX + cos(val) * tempY;
  }
}
