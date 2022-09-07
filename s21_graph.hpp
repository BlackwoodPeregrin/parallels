#ifndef SRC_S21_GRAPH_HPP_
#define SRC_S21_GRAPH_HPP_

#include <cstring>

#include "matrix/matrix.hpp"

class Graph {
 public:
  enum GraphType { ORIENTED, UNORIENTED };
  enum GraphWeights { WEIGHTED, UNWEIGHTED };

 public:
  Graph();
  Graph(const Graph &other);
  Graph(Graph &&other);
  ~Graph();

  void operator=(const Graph &other);
  void operator=(Graph &&other);

  int MinVertex() const;
  int MaxVertex() const;
  bool IsGraphInizialize() const;

  const bool &GraphType() const;
  const bool &GraphWeights() const;
  const bool &PositiveWeights() const;
  const Matrix<double> &graphMatrix() const;

  void LoadGraphFromFile(const std::string &filename);
  void ExportGraphToDot(const std::string &filename);
  void ExportGraphToTxt(const std::string &filename);

  void GenerateOrientedGraph(const int &size);
  void GenerateUnorientedGraph(const int &size, const bool &weighted);

 private:
  void InstallAdjacencyMatrix_(const int &size);
  void InstallSizeAdjacencyMatrix_(const std::string &strValue);
  void InstallRowAdjacencyMatrix_(const std::string &strValue,
                                  const int &indexRow);
  void InstallTypeGraph_();

  void SaveDigraph(const std::string &filename);
  void SaveGraph(const std::string &filename);

  double GetRandomInt();

 private:
  Matrix<double> *adjacencyMatrix_;
  bool graphType_;
  bool graphWeights_;
  bool positiveWeights_;
  unsigned random_seed_{(unsigned)time(0)};
};

#endif  // SRC_S21_GRAPH_HPP_
