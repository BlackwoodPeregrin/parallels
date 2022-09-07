#ifndef SRC_S21_GRAPH_SLGORITHMS_HPP_
#define SRC_S21_GRAPH_SLGORITHMS_HPP_

#include <ctime>
#include <mutex>
#include <set>
#include <vector>

#include "queue/s21_queue.hpp"
#include "s21_graph.hpp"
#include "stack/s21_stack.hpp"

struct TsmResult {
  std::vector<int> vertices;
  double distance;
};

class GraphAlgorithms {
  unsigned random_seed_{(unsigned)time(0)};
  std::mutex mtx_;

 public:
  /*---возвращает вектор вершин кратчайшего пути---*/
  std::vector<int> GetVectorShortestPathBetweenVertices(const Graph &graph,
                                                        const int &vertex1,
                                                        const int &vertex2);
  /*---возвращает матрицу смежности кратчайших путей между всеми вершинами---*/
  Matrix<double> GetShortestPathsBetweenAllVertices(const Graph &graph);

  /*---алгоритмы на решение задачи коммивояжера---*/
  TsmResult SolveTravelingSalesmanProblem(const Graph &graph);
  TsmResult SolveTsmBrute(const Graph &graph);
  TsmResult SolveTsmNearestNeighbour(const Graph &graph);
  TsmResult SolveTsmAcoThreaded(const Graph &graph);

 private:
  /*---приватные фунции для поиска кратчайшего пути между двумя вершинами---*/
  std::vector<int> InverseVector(const std::vector<int> &current);

  /*---приватные фунции для задачи коммивояжера---*/
  static constexpr double kEvaporationCoef = 0.8;
  TsmResult GenerateRandomPath(const Matrix<double> &paths, unsigned *seed);
  TsmResult GeneratePopularPath(const Matrix<double> &paths, unsigned *seed,
                                Matrix<double> *phero);
  void EvaporatePheromones(Matrix<double> *mx);
  void LeavingTrails(const TsmResult &path, Matrix<double> *phero);
  TsmResult InsertActualPaths(const TsmResult &data, const Graph &graph);
  int Roll(int base, unsigned *seed);
  TsmResult RecBruteSearch(int cur, std::set<int> future,
                           const Matrix<double> &paths);
  void AddPheromones(const Matrix<double> &source, Matrix<double> *target);
  void AntInitRun(int rounds, const Matrix<double> paths,
                  Matrix<double> *pheromone, unsigned seed,
                  TsmResult *short_path);
  void AntRepeatRun(int rounds, const Matrix<double> paths,
                    Matrix<double> *pheromone, unsigned seed,
                    TsmResult *short_path, int *silence);
};

#endif  // SRC_S21_GRAPH_SLGORITHMS_HPP_
