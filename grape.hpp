#pragma once

#include <iostream>
#include <map>

#include "loaderMatrix.hpp"
#include "matrix/matrix.hpp"

class GrapeAlgoritm {
 public:
  GrapeAlgoritm();
  GrapeAlgoritm(const GrapeAlgoritm &other);
  GrapeAlgoritm(GrapeAlgoritm &&other);
  ~GrapeAlgoritm();

  void operator=(const GrapeAlgoritm &other);
  void operator=(GrapeAlgoritm &&other);

  /*---Функции для перемножения матриц, переменная *weight_method* позволяет
   * искуственно загрузить метод---*/
  void CalcSingleThread(bool weight_method);
  void CalcMultiThreadClassic(bool weight_method);
  void CalcMultiThreadConveyor(bool weight_method);

  /*---Возвращает результирующую матрицу---*/
  const Matrix<float> &GetResultMatrix();

  /*---Рандомно генерирует матрицы размерностью size_matrix1 и size_matrix2---*/
  void GenerateRandomMatrix(const std::pair<size_t, size_t> &size_matrix1,
                            const std::pair<size_t, size_t> &size_matrix2);

  /*---Позволяет загурзить матрицу из файла, index_matrix оперделяет какую
   * матрицу будут загружать---*/
  void LoadMatrixFromFile(const std::string &file_name, bool index_matrix);

  bool IsMatrixLoaded(int index);
  std::pair<int, int> MatricesSizes(int index);

 private:
  /*--- Приватные функции для однопотока и класического многопотока ---*/
  void CalcRowFactor_(std::vector<float> *row_factor, bool weight_method);
  void CalcColumnFactor_(std::vector<float> *column_factor, bool weight_method);
  void CalcResultMatrix_(const std::vector<float> &row_factor,
                         const std::vector<float> &column_factor,
                         bool weight_method);
  /*--- Конвеерного Многопотока ---*/
  void CalcRowFactorConveyor_(std::map<int, float> *row_factor,
                              bool weight_method);
  void CalcColumnFactorConveyor_(std::map<int, float> *column_factor,
                                 bool weight_method);
  void CalcAllFactorsConveyor_(
      std::map<int, float> *row_factor, std::map<int, float> *column_factor,
      std::map<std::pair<int, int>, std::pair<float, float>> *value_cell_matrix,
      bool weight_method);
  void CalcResultMatrixConveyor_(
      std::map<std::pair<int, int>, std::pair<float, float>> *value_cell_matrix,
      bool weight_method);

 private:
  Matrix<float> *m_matrix1;
  Matrix<float> *m_matrix2;
  Matrix<float> *m_reult_matrix;
  LoaderMatrix *m_loader;
};
