#include "grape.hpp"

#include <ctime>
#include <queue>
#include <thread>

GrapeAlgoritm::GrapeAlgoritm()
    : m_matrix1(nullptr),
      m_matrix2(nullptr),
      m_reult_matrix(nullptr),
      m_loader(new LoaderMatrix) {}

GrapeAlgoritm::GrapeAlgoritm(const GrapeAlgoritm &other)
    : m_matrix1(nullptr),
      m_matrix2(nullptr),
      m_reult_matrix(nullptr),
      m_loader(new LoaderMatrix) {
  if (this != &other) {
    *this = other;
  }
}

GrapeAlgoritm::GrapeAlgoritm(GrapeAlgoritm &&other)
    : m_matrix1(nullptr),
      m_matrix2(nullptr),
      m_reult_matrix(nullptr),
      m_loader(new LoaderMatrix) {
  *this = std::move(other);
}

GrapeAlgoritm::~GrapeAlgoritm() {
  delete m_matrix1;
  delete m_matrix2;
  delete m_reult_matrix;
  delete m_loader;
}

void GrapeAlgoritm::operator=(const GrapeAlgoritm &other) {
  delete m_matrix1;
  delete m_matrix2;
  delete m_reult_matrix;
  delete m_loader;

  m_matrix1 = other.m_matrix1 == nullptr ? nullptr
                                         : new Matrix<float>(*other.m_matrix1);
  m_matrix2 = other.m_matrix2 == nullptr ? nullptr
                                         : new Matrix<float>(*other.m_matrix2);
  m_reult_matrix = other.m_reult_matrix == nullptr
                       ? nullptr
                       : new Matrix<float>(*other.m_reult_matrix);
  m_loader =
      other.m_loader == nullptr ? nullptr : new LoaderMatrix(*other.m_loader);
}

void GrapeAlgoritm::operator=(GrapeAlgoritm &&other) {
  if (this == &other) {
    throw std::out_of_range("Can't move yourself");
  }
  delete m_matrix1;
  delete m_matrix2;
  delete m_reult_matrix;
  delete m_loader;
  //
  m_matrix1 = nullptr;
  m_matrix2 = nullptr;
  m_reult_matrix = nullptr;
  m_loader = nullptr;
  //
  std::swap(m_matrix1, other.m_matrix1);
  std::swap(m_matrix2, other.m_matrix2);
  std::swap(m_reult_matrix, other.m_reult_matrix);
  std::swap(m_loader, other.m_loader);
}

void GrapeAlgoritm::CalcSingleThread(bool weight_method) {
  if (m_matrix1->get_columns() != m_matrix2->get_rows()) {
    throw std::invalid_argument("Can't mult matrix");
  }
  // вычисление rowFactors для первой матрицы
  std::vector<float> row_factor(m_matrix1->get_rows());
  CalcRowFactor_(&row_factor, weight_method);
  // вычисление	columnFactors для второй матрицы
  std::vector<float> column_factor(m_matrix2->get_columns());
  CalcColumnFactor_(&column_factor, weight_method);
  // вычисление результирующей матрицы
  CalcResultMatrix_(row_factor, column_factor, weight_method);
}

void GrapeAlgoritm::CalcMultiThreadClassic(bool weight_method) {
  if (m_matrix1->get_columns() != m_matrix2->get_rows()) {
    throw std::invalid_argument("Can't mult matrix");
  }
  std::vector<float> row_factor(m_matrix1->get_rows());
  std::thread th1(&GrapeAlgoritm::CalcRowFactor_, this, &row_factor,
                  weight_method);
  //
  std::vector<float> column_factor(m_matrix2->get_columns());
  std::thread th2(&GrapeAlgoritm::CalcColumnFactor_, this, &column_factor,
                  weight_method);
  //
  th1.join();
  th2.join();
  //
  CalcResultMatrix_(row_factor, column_factor, weight_method);
}

void GrapeAlgoritm::CalcMultiThreadConveyor(bool weight_method) {
  if (m_matrix1->get_columns() != m_matrix2->get_rows()) {
    throw std::invalid_argument("Can't mult matrix");
  }
  //
  std::map<int, float> row_factor;
  std::thread th1(&GrapeAlgoritm::CalcRowFactorConveyor_, this, &row_factor,
                  weight_method);
  //
  std::map<int, float> column_factor;
  std::thread th2(&GrapeAlgoritm::CalcColumnFactorConveyor_, this,
                  &column_factor, weight_method);
  //
  std::map<std::pair<int, int>, std::pair<float, float>> value_cell_matrix;
  std::thread th3(&GrapeAlgoritm::CalcAllFactorsConveyor_, this, &row_factor,
                  &column_factor, &value_cell_matrix, weight_method);
  //
  CalcResultMatrixConveyor_(&value_cell_matrix, weight_method);
  //
  th1.join();
  th2.join();
  th3.join();
}

const Matrix<float> &GrapeAlgoritm::GetResultMatrix() {
  if (m_reult_matrix == nullptr) {
    throw std::out_of_range("Result matrix nullptr");
  }
  return *m_reult_matrix;
}

void GrapeAlgoritm::GenerateRandomMatrix(
    const std::pair<size_t, size_t> &size_matrix1,
    const std::pair<size_t, size_t> &size_matrix2) {
  srand(time(NULL));
  if (m_matrix1) {
    delete m_matrix1;
  }
  m_matrix1 = new Matrix<float>(size_matrix1);
  for (int i = 0; i < m_matrix1->get_rows(); ++i) {
    for (int j = 0; j < m_matrix1->get_columns(); ++j) {
      float random_del = rand() % 10 + 1;
      (*m_matrix1)(i, j) = (float)(rand() % 1000 + 1) / random_del;
    }
  }
  if (m_matrix2) {
    delete m_matrix2;
  }
  m_matrix2 = new Matrix<float>(size_matrix2);
  for (int i = 0; i < m_matrix2->get_rows(); ++i) {
    for (int j = 0; j < m_matrix2->get_columns(); ++j) {
      float random_del = rand() % 10 + 1;
      (*m_matrix2)(i, j) = (float)(rand() % 1000 + 1) / random_del;
    }
  }
}

void GrapeAlgoritm::LoadMatrixFromFile(const std::string &file_name,
                                       bool index_matrix) {
  if (index_matrix == 0) {
    if (m_matrix1 == nullptr) {
      m_matrix1 = new Matrix<float>(m_loader->LoadFromFile(file_name));
    } else {
      *m_matrix1 = m_loader->LoadFromFile(file_name);
    }
  } else {
    if (m_matrix2 == nullptr) {
      m_matrix2 = new Matrix<float>(m_loader->LoadFromFile(file_name));
    } else {
      *m_matrix2 = m_loader->LoadFromFile(file_name);
    }
  }
}

bool GrapeAlgoritm::IsMatrixLoaded(int index) {
  if (index == 1) {
    return (m_matrix1 == nullptr) ? false : true;
  } else {
    return (m_matrix2 == nullptr) ? false : true;
  }
}

std::pair<int, int> GrapeAlgoritm::MatricesSizes(int index) {
  std::pair<int, int> res{};
  if (index == 1 && m_matrix1 != nullptr) {
    res.first = m_matrix1->get_rows();
    res.second = m_matrix1->get_columns();
  } else if (index == 2 && m_matrix2 != nullptr) {
    res.first = m_matrix2->get_rows();
    res.second = m_matrix2->get_columns();
  }
  return res;
}

void GrapeAlgoritm::CalcRowFactor_(std::vector<float> *row_factor,
                                   bool weight_method) {
  // вычисление rowFactors для первой матрицы
  int a = m_matrix1->get_rows();
  int d = m_matrix1->get_columns() / 2;
  for (int i = 0; i < a; ++i) {
    for (int j = 0, k = 0; j < d; ++j, ++k) {
      if (weight_method == true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      (*row_factor)[i] += (*m_matrix1)(i, k + j) * (*m_matrix1)(i, k + j + 1);
    }
  }
}

void GrapeAlgoritm::CalcColumnFactor_(std::vector<float> *column_factor,
                                      bool weight_method) {
  // вычисление	columnFactors для второй матрицы
  int c = m_matrix2->get_columns();
  int d = m_matrix1->get_columns() / 2;
  for (int i = 0; i < c; ++i) {
    for (int j = 0, k = 0; j < d; ++j, ++k) {
      if (weight_method == true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      (*column_factor)[i] +=
          (*m_matrix2)(k + j, i) * (*m_matrix2)(k + j + 1, i);
    }
  }
}

void GrapeAlgoritm::CalcResultMatrix_(const std::vector<float> &row_factor,
                                      const std::vector<float> &column_factor,
                                      bool weight_method) {
  // вычисление результирующей матрицы
  int a = m_matrix1->get_rows();
  int b = m_matrix1->get_columns();
  int c = m_matrix2->get_columns();
  int d = b / 2;

  if (m_reult_matrix) {
    delete m_reult_matrix;
  }
  m_reult_matrix = new Matrix<float>(a, c);

  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < c; ++j) {
      if (weight_method == true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      (*m_reult_matrix)(i, j) = -row_factor[i] - column_factor[j];
      for (int k = 0, z = 0; k < d; ++k, ++z) {
        (*m_reult_matrix)(i, j) +=
            ((*m_matrix1)(i, z + k) + (*m_matrix2)(z + k + 1, j)) *
            ((*m_matrix1)(i, z + k + 1) + (*m_matrix2)(z + k, j));
      }
      // прибавление членов в случае нечетной общей размерности
      if (b % 2 != 0) {
        (*m_reult_matrix)(i, j) +=
            (*m_matrix1)(i, b - 1) * (*m_matrix2)(b - 1, j);
      }
    }
  }
}

void GrapeAlgoritm::CalcRowFactorConveyor_(std::map<int, float> *row_factor,
                                           bool weight_method) {
  // вычисление rowFactors для первой матрицы
  int a = m_matrix1->get_rows();
  int d = m_matrix1->get_columns() / 2;
  for (int i = 0; i < a; ++i) {
    if (weight_method == true) {
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    float value_factor{};
    for (int j = 0, k = 0; j < d; ++j, ++k) {
      value_factor += (*m_matrix1)(i, k + j) * (*m_matrix1)(i, k + j + 1);
    }
    row_factor->insert({i, value_factor});
  }
}

void GrapeAlgoritm::CalcColumnFactorConveyor_(
    std::map<int, float> *column_factor, bool weight_method) {
  // вычисление	columnFactors для второй матрицы
  int c = m_matrix2->get_columns();
  int d = m_matrix1->get_columns() / 2;
  for (int i = 0; i < c; ++i) {
    if (weight_method == true) {
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    float value_factor{};
    for (int j = 0, k = 0; j < d; ++j, ++k) {
      value_factor += (*m_matrix2)(k + j, i) * (*m_matrix2)(k + j + 1, i);
    }
    column_factor->insert({i, value_factor});
  }
}

void GrapeAlgoritm::CalcAllFactorsConveyor_(
    std::map<int, float> *row_factor, std::map<int, float> *column_factor,
    std::map<std::pair<int, int>, std::pair<float, float>> *value_cell_matrix,
    bool weight_method) {
  //
  int a = m_matrix1->get_rows();
  int b = m_matrix1->get_columns();
  int c = m_matrix2->get_columns();
  int d = b / 2;
  //
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < c;) {
      if (weight_method == true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      auto value_row_factor = row_factor->find(i);
      auto column_row_factor = column_factor->find(j);
      if (value_row_factor != row_factor->end() &&
          column_row_factor != column_factor->end()) {
        float result_factor1 =
            (*value_row_factor).second + (*column_row_factor).second;
        float result_factor2{};
        for (int k = 0, z = 0; k < d; ++k, ++z) {
          result_factor2 +=
              ((*m_matrix1)(i, z + k) + (*m_matrix2)(z + k + 1, j)) *
              ((*m_matrix1)(i, z + k + 1) + (*m_matrix2)(z + k, j));
        }
        value_cell_matrix->insert({{i, j}, {result_factor2, result_factor1}});
        ++j;
      }
    }
  }
}

void GrapeAlgoritm::CalcResultMatrixConveyor_(
    std::map<std::pair<int, int>, std::pair<float, float>> *value_cell_matrix,
    bool weight_method) {
  //
  int a = m_matrix1->get_rows();
  int b = m_matrix1->get_columns();
  int c = m_matrix2->get_columns();
  //
  if (m_reult_matrix) {
    delete m_reult_matrix;
  }
  m_reult_matrix = new Matrix<float>(a, c);

  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < c;) {
      if (weight_method == true) {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      auto result_cell = value_cell_matrix->find({i, j});
      if (result_cell != value_cell_matrix->end()) {
        (*m_reult_matrix)(i, j) =
            (*result_cell).second.first - (*result_cell).second.second;
        // прибавление членов в случае нечетной общей размерности
        if (b % 2 != 0) {
          (*m_reult_matrix)(i, j) +=
              (*m_matrix1)(i, b - 1) * (*m_matrix2)(b - 1, j);
        }
        ++j;
      }
    }
  }
}
