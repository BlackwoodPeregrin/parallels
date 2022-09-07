template <class Type>
Matrix<Type>::Matrix(const int& rows, const int& columns)
    : rows_(rows), columns_(columns), matrix(nullptr) {
  if (rows < 1 || columns < 1) {
    throw std::invalid_argument("ERROR, invalid input");
  }
  matrix = new Type*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix[i] = new Type[columns_]{};
  }
}

template <class Type>
Matrix<Type>::Matrix(const std::pair<size_t, size_t>& size)
    : rows_(size.first), columns_(size.second), matrix(nullptr) {
  matrix = new Type*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix[i] = new Type[columns_]{};
  }
}

template <class Type>
Matrix<Type>::Matrix(const Matrix<Type>& other)
    : rows_(0), columns_(0), matrix(nullptr) {
  *this = other;
}

template <class Type>
Matrix<Type>::Matrix(Matrix<Type>&& other)
    : rows_(0), columns_(0), matrix(nullptr) {
  *this = std::move(other);
}

template <class Type>
Matrix<Type>::~Matrix() {
  clear();
}

template <class Type>
void Matrix<Type>::clear() {
  if (matrix != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix[i];
    }
    delete[] matrix;
    matrix = nullptr;
    rows_ = 0;
    columns_ = 0;
  }
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator=(const Matrix<Type>& other) {
  if (this == &other) {
    throw std::invalid_argument("Can't copy yourself object");
  }

  clear();
  rows_ = other.rows_;
  columns_ = other.columns_;

  matrix = new Type*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix[i] = new Type[columns_]{};
    for (int j = 0; j < columns_; j++) {
      matrix[i][j] = other.matrix[i][j];
    }
  }

  return *this;
}

template <class Type>
void Matrix<Type>::operator=(Matrix<Type>&& other) {
  if (this == &other) {
    throw std::invalid_argument("Can't move yourself object");
  }

  clear();
  std::swap(rows_, other.rows_);
  std::swap(columns_, other.columns_);
  std::swap(matrix, other.matrix);
}

template <class Type>
const int& Matrix<Type>::get_rows() const {
  return rows_;
}

template <class Type>
const int& Matrix<Type>::get_columns() const {
  return columns_;
}

template <class Type>
void Matrix<Type>::set_rows(const int& rows) {
  Matrix<Type> current(rows, columns_);
  for (int i = 0; i < current.rows_ && i < rows_; i++) {
    for (int j = 0; j < current.columns_; j++) {
      current.matrix[i][j] = matrix[i][j];
    }
  }
  *this = std::move(current);
}

template <class Type>
void Matrix<Type>::set_columns(const int& columns) {
  Matrix<Type> current(rows_, columns);
  for (int i = 0; i < current.rows_; i++) {
    for (int j = 0; j < current.columns_ && i < columns_; j++) {
      current.matrix[i][j] = matrix[i][j];
    }
  }
  *this = std::move(current);
}

template <class Type>
bool Matrix<Type>::eq_matrix(const Matrix<Type>& other) {
  if (rows_ != other.rows_ || columns_ != other.columns_) {
    return false;
  }
  for (int i = 0; i < rows_; i++) {
    if (memcmp(matrix[i], other.matrix[i], columns_ * sizeof(double)) != 0) {
      return false;
    }
  }
  return true;
}

template <class Type>
void Matrix<Type>::sum_matrix(const Matrix& other) {
  if (rows_ != other.rows_ || columns_ != other.columns_) {
    throw std::invalid_argument("ERROR, different dimensions of matrices");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_; j++) {
      matrix[i][j] += other.matrix[i][j];
    }
  }
}

template <class Type>
void Matrix<Type>::sub_matrix(const Matrix& other) {
  if (rows_ != other.rows_ || columns_ != other.columns_) {
    throw std::invalid_argument("ERROR, different dimensions of matrices");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_; j++) {
      matrix[i][j] -= other.matrix[i][j];
    }
  }
}

template <class Type>
void Matrix<Type>::mul_number(const Type& num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_; j++) {
      matrix[i][j] *= num;
    }
  }
}

template <class Type>
void Matrix<Type>::mul_matrix(const Matrix& other) {
  if (columns_ != other.rows_) {
    throw std::invalid_argument(
        "ERROR in mult matrix columns unequal rows mult matrix");
  }
  Matrix<Type> result(rows_, other.columns_);
  for (int i = 0; i < result.rows_; i++)
    for (int j = 0; j < result.columns_; j++)
      for (int k = 0; k < columns_; k++)
        result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];

  *this = std::move(result);
}

template <class Type>
Matrix<Type> Matrix<Type>::transpose() {
  Matrix<Type> newMatrix(columns_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_; j++) {
      newMatrix.matrix[j][i] = matrix[i][j];
    }
  }
  return newMatrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::operator+(const Matrix<Type>& other) {
  Matrix<Type> newMatrix(*this);
  newMatrix.sum_matrix(other);
  return newMatrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::operator-(const Matrix& other) {
  Matrix<Type> newMatrix(*this);
  newMatrix.sub_matrix(other);
  return newMatrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::operator*(const Matrix& other) {
  Matrix<Type> newMatrix(*this);
  newMatrix.mul_matrix(other);
  return newMatrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::operator*(const Type& num) {
  Matrix<Type> newMatrix(*this);
  newMatrix.mul_number(num);
  return newMatrix;
}

template <class Type>
bool Matrix<Type>::operator==(const Matrix<Type>& other) {
  return eq_matrix(other);
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator+=(const Matrix& other) {
  sum_matrix(other);
  return *this;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator-=(const Matrix& other) {
  sub_matrix(other);
  return *this;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator*=(const Matrix& other) {
  mul_matrix(other);
  return *this;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator*=(const Type& num) {
  mul_number(num);
  return *this;
}

template <class Type>
Type& Matrix<Type>::operator()(const int& row, const int& column) {
  if (row < 0 || row >= rows_ || column < 0 || column >= columns_) {
    throw std::out_of_range("ERROR index out of range");
  }
  return matrix[row][column];
}

template <class Type>
Type Matrix<Type>::operator()(const int& row, const int& column) const {
  if (row < 0 || row >= rows_ || column < 0 || column >= columns_) {
    throw std::out_of_range("ERROR index out of range");
  }
  return matrix[row][column];
}
