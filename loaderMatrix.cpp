#include "loaderMatrix.hpp"

#include <fstream>

LoaderMatrix::LoaderMatrix() : m_matrix(nullptr) {}

LoaderMatrix::LoaderMatrix(const LoaderMatrix &other) : m_matrix(nullptr) {
  if (this != &other) {
    *this = other;
  }
}

LoaderMatrix::LoaderMatrix(LoaderMatrix &&other) : m_matrix(nullptr) {
  *this = std::move(other);
}

LoaderMatrix::~LoaderMatrix() { delete m_matrix; }

void LoaderMatrix::operator=(const LoaderMatrix &other) {
  delete m_matrix;
  m_matrix =
      other.m_matrix == nullptr ? nullptr : new Matrix<float>(*other.m_matrix);
}

void LoaderMatrix::operator=(LoaderMatrix &&other) {
  if (this == &other) {
    throw std::out_of_range("Can't move yourself");
  }
  delete m_matrix;
  m_matrix = nullptr;
  std::swap(m_matrix, other.m_matrix);
}

const Matrix<float> &LoaderMatrix::LoadFromFile(const std::string &file_name) {
  std::ifstream stream(file_name);
  if (stream.is_open()) {
    std::string line{};
    std::getline(stream, line);
    if (InstallSizeMatrix_(line) == false) {
      throw std::invalid_argument("Incorrect load File.");
    }
    int index_row = 0;
    while (!stream.eof()) {
      std::getline(stream, line);
      if (!line.empty()) {
        if (InstallRowMatrix_(line, index_row) == false) {
          throw std::invalid_argument("Incorrect load File.");
        }
        ++index_row;
      }
    }
    if (index_row != m_matrix->get_rows()) {
      throw std::invalid_argument("Incorrect load File.");
    }
    stream.close();
  } else {
    throw std::invalid_argument("Error, no such file - " + file_name);
  }
  return *m_matrix;
}

bool LoaderMatrix::InstallSizeMatrix_(const std::string &str_from_file) {
  auto result = ReadNumbersFromStrFile(str_from_file);
  std::vector<int> values;
  if (result.second == false) {
    return false;
  }
  for (size_t i = 0; i < result.first.size(); ++i) {
    if (IsUnsignedInt_(result.first[i]) == true) {
      try {
        values.push_back(std::stoi(result.first[i]));
      } catch (const std::exception &e) {
        return false;
      }
    } else {
      return false;
    }
  }
  if (values.size() != 2) {
    return false;
  }
  delete m_matrix;
  m_matrix = new Matrix<float>(values.front(), values.back());
  //
  return true;
}

bool LoaderMatrix::InstallRowMatrix_(const std::string &str_from_file,
                                     const int &index_row) {
  if (index_row >= m_matrix->get_rows()) {
    return false;
  }
  auto result = ReadNumbersFromStrFile(str_from_file);
  std::vector<float> values;
  if (result.second == true) {
    for (size_t i = 0; i < result.first.size(); ++i) {
      try {
        values.push_back(std::stod(result.first[i]));
      } catch (const std::exception &e) {
        return false;
      }
    }
    if (values.size() != (size_t)m_matrix->get_columns()) {
      return false;
    }
    for (size_t column = 0; column < values.size(); ++column) {
      (*m_matrix)(index_row, column) = values[column];
    }
  } else {
    return false;
  }
  return true;
}

std::pair<std::vector<std::string>, bool> LoaderMatrix::ReadNumbersFromStrFile(
    const std::string &str_from_file) {
  bool result = true;
  bool skip_space = true;
  std::vector<std::string> values{};
  std::string number{};
  for (size_t i = 0; i < str_from_file.size(); ++i) {
    if (str_from_file[i] == ' ' && skip_space == false) {
      values.push_back(number);
      number.clear();
      skip_space = true;
    } else if (i == str_from_file.size() - 1) {
      if (str_from_file[i] != ' ') {
        number.push_back(str_from_file[i]);
        values.push_back(number);
      }
    } else if ((str_from_file[i] >= '0' && str_from_file[i] <= '9') ||
               str_from_file[i] == '.' || str_from_file[i] == '-') {
      number.push_back(str_from_file[i]);
      skip_space = false;
    } else {
      if (str_from_file[i] != ' ') {
        result = false;
        break;
      }
    }
  }
  return {values, result};
}

bool LoaderMatrix::IsUnsignedInt_(const std::string &value) {
  for (size_t i = 0; i < value.size(); ++i) {
    if (value[i] < '0' || value[i] > '9') {
      return false;
    }
  }
  return true;
}
