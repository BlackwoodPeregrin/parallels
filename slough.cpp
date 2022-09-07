#include "slough.hpp"

#include <fstream>
#include <thread>

CalcSlough::CalcSlough() : m_matrix(nullptr), m_loader(new LoaderMatrix) {}

CalcSlough::CalcSlough(const CalcSlough &other)
    : m_matrix(nullptr), m_loader(new LoaderMatrix) {
  if (this != &other) {
    *this = other;
  }
}

CalcSlough::CalcSlough(CalcSlough &&other)
    : m_matrix(nullptr), m_loader(new LoaderMatrix) {
  *this = std::move(other);
}

CalcSlough::~CalcSlough() {
  delete m_matrix;
  delete m_loader;
}

void CalcSlough::operator=(const CalcSlough &other) {
  delete m_matrix;
  delete m_loader;
  m_matrix =
      other.m_matrix == nullptr ? nullptr : new Matrix<float>(*other.m_matrix);
  m_loader =
      other.m_loader == nullptr ? nullptr : new LoaderMatrix(*other.m_loader);
}

void CalcSlough::operator=(CalcSlough &&other) {
  if (this == &other) {
    throw std::out_of_range("Can't move yourself");
  }
  delete m_matrix;
  delete m_loader;
  m_matrix = nullptr;
  m_loader = nullptr;
  std::swap(m_matrix, other.m_matrix);
  std::swap(m_loader, other.m_loader);
}

/*=== Функция для решения СЛАУ, переменная *weight_method* позволяет
 * искуственно нагрузить метод ===*/

void CalcSlough::CalcSloughtSingleThread(bool weight_method) {
  int rows = m_matrix->get_rows();
  int columns = m_matrix->get_columns() - 1;
  for (int j = 0; j < columns; ++j) {
    for (int i = 0; i < rows; ++i) {
      if (weight_method == true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      LogicAlgoritmToTriangleMatrix_(i, j);
    }
  }
}

void CalcSlough::CalcSloughtMultiThread(bool weight_method) {
  int rows = m_matrix->get_rows();
  int columns = m_matrix->get_columns() - 1;

  std::thread *th1 = nullptr;
  std::thread *th2 = nullptr;

  bool th2_start = false;
  for (int j = 0; j < columns; ++j) {
    for (int i = 0; i < rows; ++i) {
      if (weight_method == true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      th1 = new std::thread(&CalcSlough::LogicAlgoritmToTriangleMatrix_, this,
                            i, j);
      if (i != rows - 1) {
        i++;
        th2 = new std::thread(&CalcSlough::LogicAlgoritmToTriangleMatrix_, this,
                              i, j);
        th2_start = true;
      }
      th1->join();
      if (th2_start) {
        th2->join();
      }
      delete th1;
      if (th2_start) {
        delete th2;
        th2_start = false;
      }
    }
  }
}

std::vector<float> CalcSlough::GetResultSlough() {
  if (m_matrix == nullptr) {
    throw std::out_of_range("Slough is not inizialize");
  }
  std::vector<float> result;
  for (int i = 0; i < m_matrix->get_rows(); ++i) {
    result.push_back((*m_matrix)(i, m_matrix->get_columns() - 1));
  }
  return result;
}

/*=== Функция для считывания матрицы СЛАУ из файла ===*/

void CalcSlough::LoadSloughFromFile(const std::string &file_name) {
  if (m_matrix == nullptr) {
    m_matrix = new Matrix<float>(m_loader->LoadFromFile(file_name));
  } else {
    *m_matrix = m_loader->LoadFromFile(file_name);
  }
  if (m_matrix->get_rows() != m_matrix->get_columns() - 1) {
    throw std::invalid_argument("Error, Incorrect slough");
  }
}

/*=== Приватные функции для приведения матрицы к треугольному виду ===*/

void CalcSlough::LogicAlgoritmToTriangleMatrix_(int row, int column) {
  if (row == column) {
    // если равняется 0, проверяем есть ли возможность поменять строчки
    if ((*m_matrix)(row, column) == 0.0) {
      for (int k = row + 1; k < m_matrix->get_rows(); ++k) {
        if ((*m_matrix)(k, column) != 0.0) {
          SwapRowMatrix_(row, k);
          break;
        }
      }
    }
    //
    if ((*m_matrix)(row, column) != 1.0) {
      float del = (*m_matrix)(row, column);
      DelRowMatrix_(row, del);
    }
    //
  } else {
    float factor =
        ((*m_matrix)(row, column) / (*m_matrix)(column, column)) * -1.0;
    ZeroRowMatrix_(row, column, factor);
  }
}

void CalcSlough::SwapRowMatrix_(int first_row, int second_row) {
  for (int i = 0; i < m_matrix->get_columns(); ++i) {
    float temp = (*m_matrix)(first_row, i);
    (*m_matrix)(first_row, i) = (*m_matrix)(second_row, i);
    (*m_matrix)(second_row, i) = temp;
  }
}

void CalcSlough::DelRowMatrix_(int index_row, float del) {
  for (int i = 0; i < m_matrix->get_columns(); ++i) {
    (*m_matrix)(index_row, i) /= del;
  }
}

void CalcSlough::ZeroRowMatrix_(int cur_index_row, int added_index_row,
                                float factor) {
  for (int i = 0; i < m_matrix->get_columns(); ++i) {
    float value = (*m_matrix)(added_index_row, i) * factor;
    (*m_matrix)(cur_index_row, i) += value;
  }
}

bool CalcSlough::IsMatrixLoaded() { return (m_matrix != nullptr); }
