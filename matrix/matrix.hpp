#pragma once

#include <cmath>
#include <iostream>

template <class Type>
class Matrix {
 public:
  Matrix(const int& rows, const int& columns);
  Matrix(const std::pair<size_t, size_t>& size);
  Matrix(const Matrix& other);
  Matrix(Matrix&& other);
  ~Matrix();

  Matrix& operator=(const Matrix& other);
  void operator=(Matrix&& other);

  /* accessors */
  const int& get_rows() const;
  const int& get_columns() const;
  /* mutators */
  void set_rows(const int& rows);
  void set_columns(const int& columns);

  bool eq_matrix(const Matrix& other);
  void sum_matrix(const Matrix& other);
  void sub_matrix(const Matrix& other);
  void mul_number(const Type& num);
  void mul_matrix(const Matrix& other);
  Matrix transpose();

  Matrix operator+(const Matrix& other);
  Matrix operator-(const Matrix& other);
  Matrix operator*(const Matrix& other);
  Matrix operator*(const Type& num);

  bool operator==(const Matrix& other);
  Matrix& operator+=(const Matrix& other);
  Matrix& operator-=(const Matrix& other);
  Matrix& operator*=(const Matrix& other);
  Matrix& operator*=(const Type& num);
  Type& operator()(const int& row, const int& column);
  Type operator()(const int& row, const int& column) const;

 private:
  int rows_;
  int columns_;
  Type** matrix;
  void clear();
};

#include "matrix.inl"
