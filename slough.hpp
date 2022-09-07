#pragma once

#include <cstring>
#include <iostream>
#include <vector>

#include "loaderMatrix.hpp"
#include "matrix/matrix.hpp"

class CalcSlough {
 public:
  CalcSlough();
  CalcSlough(const CalcSlough &other);
  CalcSlough(CalcSlough &&other);
  ~CalcSlough();
  void operator=(const CalcSlough &other);
  void operator=(CalcSlough &&other);

  /*---Функции для решения СЛАУ, переменная *weight_method* позволяет
   * искуственно нагрузить метод---*/
  void CalcSloughtSingleThread(bool weight_method);
  void CalcSloughtMultiThread(bool weight_method);
  std::vector<float> GetResultSlough();
  /*---функция для считывания матрицы СЛАУ из файла---*/
  void LoadSloughFromFile(const std::string &file_name);

  bool IsMatrixLoaded();

 private:
  /*---приватные функции для приведения матрицы к треугольному виду---*/
  void LogicAlgoritmToTriangleMatrix_(int row, int column);
  void SwapRowMatrix_(int first_row, int second_row);
  void DelRowMatrix_(int index_row, float del);
  void ZeroRowMatrix_(int cur_index_row, int added_index_row, float factor);

 private:
  Matrix<float> *m_matrix;
  LoaderMatrix *m_loader;
};
