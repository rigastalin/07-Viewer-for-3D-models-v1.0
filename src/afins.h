#ifndef SRC_AFINS_H_
#define SRC_AFINS_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
matrix_t s21_mult_number(matrix_t *A, double number);
void scale(data_struct *data, double val);
void moveX(data_struct *data, double val);
void moveY(data_struct *data, double val);
void moveZ(data_struct *data, double val);
void rotationX(data_struct *data, double val);
void rotationY(data_struct *data, double val);
void rotationZ(data_struct *data, double val);

#endif  // SRC_AFINS_H_
