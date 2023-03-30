#include <check.h>

#include "afins.h"
#include "parse.h"

void s21_remove_struct_matrix(data_struct *data) {
  for (int i = 0; i < data->count_V; ++i) {
    s21_remove_matrix(&data->matrix_3d[i]);
  }
  free(data->matrix_3d);
  data->count_V = 0;
}

START_TEST(s21_3d_test) {
  int rows = 8;
  int columns = 3;
  // int rowsf = 12;
  int columnsf = 3;
  matrix_t V = s21_create_matrix(rows, columns);
  V.matrix[0][0] = 1.0;
  V.matrix[0][1] = -1.0;
  V.matrix[0][2] = -1.0;
  V.matrix[1][0] = 1.0;
  V.matrix[1][1] = -1.0;
  V.matrix[1][2] = 1.0;
  V.matrix[2][0] = -1.0;
  V.matrix[2][1] = -1.0;
  V.matrix[2][2] = 1.0;
  V.matrix[3][0] = -1.0;
  V.matrix[3][1] = -1.0;
  V.matrix[3][2] = -1.0;
  V.matrix[4][0] = 1.0;
  V.matrix[4][1] = 1.0;
  V.matrix[4][2] = -0.999999;
  V.matrix[5][0] = 0.999999;
  V.matrix[5][1] = 1.0;
  V.matrix[5][2] = 1.000001;
  V.matrix[6][0] = -1.0;
  V.matrix[6][1] = 1.0;
  V.matrix[6][2] = 1.0;
  V.matrix[7][0] = -1.0;
  V.matrix[7][1] = 1.0;
  V.matrix[7][2] = -1.0;
  data_struct data;
  data.count_F = 0;
  data.count_V = 0;
  data.count_EDGES = 0;

  FILE *file = fopen("./objects/cube_test2.obj", "r");
  counters_v(&data, file);
  counters_f(&data, file);
  matrixFill(&data, file);
  polygonFill(&data, file);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      char s21[128];
      char orig[128];
      snprintf(orig, sizeof(orig), "%lf", V.matrix[i][j]);
      snprintf(s21, sizeof(s21), "%lf", data.matrix_3d[i].matrix[j][0]);
      ck_assert_str_eq(orig, s21);
    }
  }
  fclose(file);
  double val = 3.0;
  scale(&data, val);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      char s21[128];
      char orig[128];
      V.matrix[i][j] *= val;
      snprintf(orig, sizeof(orig), "%lf", V.matrix[i][j]);
      snprintf(s21, sizeof(s21), "%lf", data.matrix_3d[i].matrix[j][0]);
      ck_assert_str_eq(orig, s21);
    }
  }
  moveX(&data, val);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      char s21[128];
      char orig[128];
      if (j == 0) V.matrix[i][j] = V.matrix[i][j] + val;
      snprintf(orig, sizeof(orig), "%lf", V.matrix[i][j]);
      snprintf(s21, sizeof(s21), "%lf", data.matrix_3d[i].matrix[j][0]);
      ck_assert_str_eq(orig, s21);
    }
  }
  moveY(&data, val);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      char s21[128];
      char orig[128];
      if (j == 1) V.matrix[i][j] = V.matrix[i][j] + val;
      snprintf(orig, sizeof(orig), "%lf", V.matrix[i][j]);
      snprintf(s21, sizeof(s21), "%lf", data.matrix_3d[i].matrix[j][0]);
      ck_assert_str_eq(orig, s21);
    }
  }
  moveZ(&data, val);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      char s21[128];
      char orig[128];
      if (j == 2) V.matrix[i][j] = V.matrix[i][j] + val;
      snprintf(orig, sizeof(orig), "%lf", V.matrix[i][j]);
      snprintf(s21, sizeof(s21), "%lf", data.matrix_3d[i].matrix[j][0]);
      ck_assert_str_eq(orig, s21);
    }
  }
  rotationX(&data, val);
  rotationY(&data, val);
  rotationZ(&data, val);
  ck_assert_int_eq(data.count_V, rows);
  ck_assert_int_eq(data.polygons->numofone, 2 * columnsf);

  for (int i = 0; i < data.count_F; i++) {
    free(data.polygons[i].vertexes);
    data.polygons[i].numofone = 0;
  }
  free(data.polygons);

  s21_remove_matrix(&V);
  s21_remove_struct_matrix(&data);
}
END_TEST

int main(void) {
  Suite *s_s21_3d = suite_create("s21_3d");

  TCase *tc_s21_3d = tcase_create("s21_3d_test");

  SRunner *sr = srunner_create(s_s21_3d);

  suite_add_tcase(s_s21_3d, tc_s21_3d);
  tcase_add_test(tc_s21_3d, s21_3d_test);

  srunner_run_all(sr, CK_ENV);
  srunner_ntests_failed(sr);
  srunner_free(sr);
}
