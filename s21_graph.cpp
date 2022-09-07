#include "s21_graph.hpp"

#include <cstring>
#include <fstream>
#include <set>
#include <vector>

Graph::Graph() : adjacencyMatrix_(nullptr) {}

Graph::Graph(const Graph &other) : adjacencyMatrix_(nullptr) { *this = other; }

Graph::Graph(Graph &&other) : adjacencyMatrix_(nullptr) {
  *this = std::move(other);
}

Graph::~Graph() { delete adjacencyMatrix_; }

void Graph::operator=(const Graph &other) {
  if (this != &other) {
    if (IsGraphInizialize()) {
      delete adjacencyMatrix_;
      adjacencyMatrix_ = nullptr;
    }
    if (other.IsGraphInizialize()) {
      int mx_size = other.adjacencyMatrix_->get_rows();
      adjacencyMatrix_ = new Matrix<double>(mx_size, mx_size);
      adjacencyMatrix_ = other.adjacencyMatrix_;
    }
    graphType_ = other.graphType_;
    graphWeights_ = other.graphWeights_;
    positiveWeights_ = other.positiveWeights_;
  }
}

void Graph::operator=(Graph &&other) {
  if (this == &other) {
    throw std::out_of_range("Can't move yourself");
  }
  if (IsGraphInizialize()) {
    delete adjacencyMatrix_;
    adjacencyMatrix_ = nullptr;
  }
  std::swap(adjacencyMatrix_, other.adjacencyMatrix_);
  std::swap(graphType_, other.graphType_);
  std::swap(graphWeights_, other.graphWeights_);
  std::swap(positiveWeights_, other.positiveWeights_);
}

int Graph::MinVertex() const {
  if (!IsGraphInizialize()) {
    throw std::out_of_range("Graph not inizialize.");
  }
  return 1;
}

int Graph::MaxVertex() const {
  if (!IsGraphInizialize()) {
    throw std::out_of_range("Graph not inizialize.");
  }
  return adjacencyMatrix_->get_rows();
}

bool Graph::IsGraphInizialize() const {
  return adjacencyMatrix_ != nullptr ? true : false;
}

const bool &Graph::GraphType() const {
  if (!IsGraphInizialize()) {
    throw std::out_of_range("Graph not inizialize");
  }
  return graphType_;
}

const bool &Graph::GraphWeights() const {
  if (!IsGraphInizialize()) {
    throw std::out_of_range("Graph not inizialize.");
  }
  return graphWeights_;
}

const bool &Graph::PositiveWeights() const {
  if (!IsGraphInizialize()) {
    throw std::out_of_range("Graph not inizialize.");
  }
  return positiveWeights_;
}

const Matrix<double> &Graph::graphMatrix() const {
  if (!IsGraphInizialize()) {
    throw std::out_of_range("Graph not inizialize.");
  }
  return *adjacencyMatrix_;
}

void Graph::LoadGraphFromFile(const std::string &filename) {
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::string line{};
    std::getline(stream, line);
    InstallSizeAdjacencyMatrix_(line);
    int indexRow = 0;
    while (!stream.eof()) {
      std::getline(stream, line);
      if (line.empty()) {
        continue;
      }
      InstallRowAdjacencyMatrix_(line, indexRow);
      ++indexRow;
    }
    if (indexRow != adjacencyMatrix_->get_rows()) {
      throw std::invalid_argument(
          "Incorrect load File. Try choose another file.");
    }
    InstallTypeGraph_();
    stream.close();
  } else {
    throw std::out_of_range("No such file.");
  }
}

void Graph::ExportGraphToDot(const std::string &filename) {
  if (!IsGraphInizialize()) {
    throw std::out_of_range(
        "Graph not inizialize. Inizialize graph before export!");
  }
  if (graphType_ == GraphType::ORIENTED) {
    SaveDigraph(filename);
  } else {
    SaveGraph(filename);
  }
}

void Graph::ExportGraphToTxt(const std::string &filename) {
  if (!IsGraphInizialize()) {
    throw std::out_of_range(
        "Graph not inizialize. Inizialize graph before export!");
  }
  std::ofstream stream(filename);
  if (stream.is_open()) {
    stream << adjacencyMatrix_->get_rows() << std::endl;
    for (int i = 0; i < adjacencyMatrix_->get_rows(); ++i) {
      for (int j = 0; j < adjacencyMatrix_->get_columns(); ++j) {
        stream << (*adjacencyMatrix_)(i, j) << " ";
      }
      stream << std::endl;
    }
    stream.close();
  } else {
    throw std::out_of_range("File not open.");
  }
}

void Graph::GenerateOrientedGraph(const int &size) {
  if (size <= 1) {
    throw std::out_of_range("Graph size must be '>1'.");
  }
  graphType_ = GraphType::ORIENTED;
  graphWeights_ = GraphWeights::WEIGHTED;
  positiveWeights_ = true;
  //
  delete adjacencyMatrix_;
  adjacencyMatrix_ = new Matrix<double>(size, size);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (i != j) {
        bool addEdge = rand_r(&random_seed_) % 2;
        if (addEdge) {
          (*adjacencyMatrix_)(i, j) = GetRandomInt();
        }
      }
    }
  }
}

void Graph::GenerateUnorientedGraph(const int &size, const bool &weighted) {
  if (size <= 1) {
    throw std::out_of_range("Graph size must be '>1'.");
  }
  graphType_ = GraphType::UNORIENTED;
  if (weighted == true) {
    graphWeights_ = GraphWeights::WEIGHTED;
  } else {
    graphWeights_ = GraphWeights::UNWEIGHTED;
  }
  positiveWeights_ = true;
  //
  delete adjacencyMatrix_;
  adjacencyMatrix_ = new Matrix<double>(size, size);
  std::set<std::pair<int, int>> vertex;
  for (int i = 0; i < size; ++i) {
    int sumEdgesVertex = 0;
    for (int j = 0; j < size; ++j) {
      if (vertex.find({j, i}) == vertex.end() && (i != j)) {
        bool addEdge = rand_r(&random_seed_) % 2;
        if (addEdge == true || ((j == size - 1) && sumEdgesVertex == 0)) {
          ++sumEdgesVertex;
          int valueEdge;
          if (weighted == true) {
            valueEdge = GetRandomInt();
          } else {
            valueEdge = 1.0;
          }
          (*adjacencyMatrix_)(i, j) = valueEdge;
          (*adjacencyMatrix_)(j, i) = valueEdge;
          vertex.insert({j, i});
        }
      }
    }
  }
}

void Graph::InstallAdjacencyMatrix_(const int &size) {
  if (IsGraphInizialize()) {
    delete adjacencyMatrix_;
  }
  adjacencyMatrix_ = new Matrix<double>(size, size);
}

void Graph::InstallSizeAdjacencyMatrix_(const std::string &strValue) {
  bool correct = true;
  for (size_t i = 0; i < strValue.size(); ++i) {
    if (strValue[i] < '1' || strValue[i] > '9') {
      correct = false;
    }
  }
  if (correct == false) {
    throw std::invalid_argument(
        "Incorrect load File. Try choose another file.");
  }
  InstallAdjacencyMatrix_(std::stoi(strValue));
}

void Graph::InstallRowAdjacencyMatrix_(const std::string &strValue,
                                       const int &indexRow) {
  if (indexRow >= adjacencyMatrix_->get_rows()) {
    throw std::invalid_argument(
        "Incorrect load File. Try choose another file.");
  }
  std::string value{};
  int indexColumn = 0;
  for (size_t i = 0; i < strValue.size(); ++i) {
    if (indexColumn >= adjacencyMatrix_->get_columns()) {
      throw std::invalid_argument(
          "Incorrect load File. Try choose another file.");
    }
    if (strValue[i] == ' ' && !value.empty()) {
      (*adjacencyMatrix_)(indexRow, indexColumn) = std::stod(value);
      value.clear();
      ++indexColumn;
    } else if (i == strValue.size() - 1) {
      value.push_back(strValue[i]);
      (*adjacencyMatrix_)(indexRow, indexColumn) = std::stod(value);
      ++indexColumn;
    } else {
      if (strValue[i] != ' ') {
        value.push_back(strValue[i]);
      }
    }
  }
  if (indexColumn != adjacencyMatrix_->get_columns()) {
    throw std::invalid_argument(
        "Incorrect load File. Try choose another file.");
  }
}

void Graph::InstallTypeGraph_() {
  /*---default---*/
  graphType_ = GraphType::UNORIENTED;
  graphWeights_ = GraphWeights::UNWEIGHTED;
  positiveWeights_ = true;
  /*-------------*/
  for (int i = 0; i < adjacencyMatrix_->get_rows(); ++i) {
    for (int j = 0; j < adjacencyMatrix_->get_columns(); ++j) {
      if ((*adjacencyMatrix_)(i, j) != 0) {
        if ((*adjacencyMatrix_)(i, j) > 1) {
          graphWeights_ = GraphWeights::WEIGHTED;
        } else if ((*adjacencyMatrix_)(i, j) < 0) {
          graphWeights_ = GraphWeights::WEIGHTED;
          positiveWeights_ = false;
        }
        if ((*adjacencyMatrix_)(j, i) != (*adjacencyMatrix_)(i, j)) {
          graphType_ = GraphType::ORIENTED;
        }
      }
    }
  }
}

void Graph::SaveDigraph(const std::string &filename) {
  std::ofstream stream(filename);
  if (stream.is_open()) {
    stream << "digraph {" << std::endl;
    std::string connection = " -> ";
    for (int i = 0; i < adjacencyMatrix_->get_rows(); ++i) {
      for (int j = 0; j < adjacencyMatrix_->get_columns(); ++j) {
        if ((*adjacencyMatrix_)(i, j) != 0) {
          if (graphWeights_ == GraphWeights::WEIGHTED) {
            double valueWeight = (*adjacencyMatrix_)(i, j);
            std::string weight{};
            if (valueWeight - (int)valueWeight == 0) {
              weight = " [label = " + std::to_string((int)valueWeight) + "];";
            } else {
              weight = " [label = " + std::to_string(valueWeight) + "];";
            }
            stream << "    " << i + 1 << connection << j + 1 << weight
                   << std::endl;
          } else {
            stream << "    " << i + 1 << connection << j + 1 << ";"
                   << std::endl;
          }
        }
      }
    }
    stream << "}" << std::endl;
    stream.close();
  } else {
    throw std::out_of_range("File not open.");
  }
}

void Graph::SaveGraph(const std::string &filename) {
  std::ofstream stream(filename);
  if (stream.is_open()) {
    stream << "graph {" << std::endl;
    std::string connection = " -- ";
    std::vector<std::pair<int, int>> sameNode;
    bool skip = false;
    for (int i = 0; i < adjacencyMatrix_->get_rows(); ++i) {
      for (int j = 0; j < adjacencyMatrix_->get_columns(); ++j) {
        if ((*adjacencyMatrix_)(i, j) != 0) {
          //
          for (size_t k = 0; k < sameNode.size(); ++k) {
            std::pair<int, int> node = sameNode[k];
            if (node.first == i && node.second == j) {
              skip = true;
              break;
            }
          }
          //
          if (skip == false) {
            if (graphWeights_ == GraphWeights::WEIGHTED) {
              double valueWeight = (*adjacencyMatrix_)(i, j);
              std::string weight{};
              if (valueWeight - (int)valueWeight == 0) {
                weight = " [label = " + std::to_string((int)valueWeight) + "];";
              } else {
                weight = " [label = " + std::to_string(valueWeight) + "];";
              }
              stream << "    " << i + 1 << connection << j + 1 << weight
                     << std::endl;
            } else {
              stream << "    " << i + 1 << connection << j + 1 << ";"
                     << std::endl;
            }
          }
          sameNode.push_back({j, i});
          skip = false;
        }
      }
    }
    stream << "}" << std::endl;
    stream.close();
  } else {
    throw std::out_of_range("File not open.");
  }
}

double Graph::GetRandomInt() {
  double randomDel = rand_r(&random_seed_) % 10 + 1;
  double randomValueEdge = (rand_r(&random_seed_) % 1000 + 1) / randomDel;
  return randomValueEdge;
}
