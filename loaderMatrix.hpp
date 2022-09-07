#pragma once

#include <iostream>
#include <vector>

#include "matrix/matrix.hpp"

class LoaderMatrix {
 public:
  LoaderMatrix();
  LoaderMatrix(const LoaderMatrix &other);
  LoaderMatrix(LoaderMatrix &&other);
  ~LoaderMatrix();

  void operator=(const LoaderMatrix &other);
  void operator=(LoaderMatrix &&other);

  const Matrix<float> &LoadFromFile(const std::string &file_name);

 private:
  /*---приватные функции для считывания матрицы СЛАУ из файла---*/
  bool InstallSizeMatrix_(const std::string &str_from_file);
  bool InstallRowMatrix_(const std::string &str_from_file,
                         const int &index_row);
  std::pair<std::vector<std::string>, bool> ReadNumbersFromStrFile(
      const std::string &str_from_file);
  bool IsUnsignedInt_(const std::string &value);

 private:
  Matrix<float> *m_matrix;
};
