#include "s21_graph_algorithms.hpp"

#include <stdlib.h>

#include <array>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <map>
#include <thread>

std::vector<int> GraphAlgorithms::GetVectorShortestPathBetweenVertices(
    const Graph &graph, const int &vertex1, const int &vertex2) {
  if (vertex1 > graph.MaxVertex() || vertex1 < graph.MinVertex() ||
      vertex2 > graph.MaxVertex() || vertex2 < graph.MinVertex()) {
    throw std::invalid_argument("Vertexes out of range graph.");
  }

  if (graph.PositiveWeights() == false) {
    throw std::invalid_argument("Weigts grath is negative");
  }

  /*---текущая вершина---*/
  int curVertex = vertex1;
  /*---кратчайший путь до текущей вершины---*/
  double curLenPath = 0;
  /*---переменная для хранения бесконечности---*/
  double inf = std::numeric_limits<double>::infinity();
  /*---матрица смежности входящего графа---*/
  Matrix<double> dataGraph(graph.graphMatrix());

  /*---key - посещенная вершина, value - кратчайший путь до нее---*/
  std::set<int> visitedVertex{curVertex};
  /*---key - вершина, value - родитель вершины по кратчайшему пути---*/
  std::map<int, int> visitedVertexAndParent;
  for (int i = 0; i < dataGraph.get_columns(); ++i) {
    int vertex = i + 1;
    if (vertex == vertex1) {
      /*---стартовая вершина является роидителем самой себе---*/
      visitedVertexAndParent.insert({vertex, vertex});
    } else {
      /*---родитель пока что не установлен---*/
      visitedVertexAndParent.insert({vertex, inf});
    }
  }

  /*---временные значения расстояний до всех вершин---*/
  std::vector<double> searchPath;

  for (int j = 0; j < dataGraph.get_columns(); ++j) {
    if (j == curVertex - 1) {
      searchPath.push_back(0);
    } else {
      searchPath.push_back(inf);  // пути между вершинами нет
    }
  }

  while (true) {
    for (size_t i = 0; i < searchPath.size(); ++i) {
      int vertex = i + 1;
      if (visitedVertex.find(vertex) == visitedVertex.end()) {
        if (dataGraph(curVertex - 1, vertex - 1) != 0) {
          double possibleLenPath =
              curLenPath + dataGraph(curVertex - 1, vertex - 1);
          if (possibleLenPath < searchPath[i]) {
            searchPath[i] = possibleLenPath;
            /*---обновляем родителя для текущей вершины---*/
            visitedVertexAndParent[vertex] = curVertex;
          }
        }
      }
    }
    //
    std::map<int, double> unique;
    for (size_t i = 0; i < searchPath.size(); ++i) {
      int vertex = i + 1;
      double valuePath = searchPath[i];
      if (valuePath != inf &&
          visitedVertex.find(vertex) == visitedVertex.end()) {
        unique.insert({vertex, valuePath});
      }
    }
    //
    if (unique.size() == 0) {
      break;  // путей больше нет
    }
    //
    auto iter = unique.begin();
    curVertex = (*iter).first;
    curLenPath = (*iter).second;
    for (++iter; iter != unique.end(); ++iter) {
      if (curLenPath > (*iter).second) {
        curVertex = (*iter).first;
        curLenPath = (*iter).second;
      }
    }
    //
    visitedVertex.insert(curVertex);
    /*---если нашли минимальный путь не обходя все вершины то выходим из
     * цикла---*/
    if (visitedVertex.find(vertex2) != visitedVertex.end()) {
      break;
    }
  }

  if (visitedVertex.find(vertex2) == visitedVertex.end()) {
    throw std::invalid_argument(
        "Can't build path beetween vertexes: " + std::to_string(vertex1) +
        " -> " + std::to_string(vertex2));
  }

  std::vector<int> shortestPath;
  auto iter1 = visitedVertexAndParent.find(vertex2);
  while ((*iter1).first != vertex1) {
    shortestPath.push_back((*iter1).first);
    iter1 = visitedVertexAndParent.find((*iter1).second);
  }
  shortestPath.push_back(vertex1);

  return InverseVector(shortestPath);
}

std::vector<int> GraphAlgorithms::InverseVector(
    const std::vector<int> &current) {
  std::vector<int> inverseVector;
  for (int i = current.size() - 1; i >= 0; --i) {
    inverseVector.push_back(current[i]);
  }
  return inverseVector;
}

Matrix<double> GraphAlgorithms::GetShortestPathsBetweenAllVertices(
    const Graph &graph) {
  if (graph.PositiveWeights() == false) {
    throw std::out_of_range("weigts grath is negative");
  }

  /*---матрица смежности входящего графа---*/
  Matrix<double> dataGraph(graph.graphMatrix());

  /*---заменяем значения 0 если пути нет на бесконечность---*/
  for (int i = 0; i < dataGraph.get_rows(); ++i) {
    for (int j = 0; j < dataGraph.get_columns(); ++j) {
      if (dataGraph(i, j) == 0 && (i != j)) {
        dataGraph(i, j) = std::numeric_limits<double>::infinity();
      }
    }
  }

  for (int k = 0; k < dataGraph.get_rows(); ++k) {
    for (int i = 0; i < dataGraph.get_rows(); ++i) {
      for (int j = 0; j < dataGraph.get_columns(); ++j) {
        if ((dataGraph(i, j) > dataGraph(i, k) + dataGraph(k, j))) {
          dataGraph(i, j) = dataGraph(i, k) + dataGraph(k, j);
        }
      }
    }
  }
  return dataGraph;
}

// ========== 4: Travelling salesman problem ==========

TsmResult GraphAlgorithms::SolveTravelingSalesmanProblem(const Graph &graph) {
  TsmResult res{};
  int mx_size = graph.MaxVertex();
  Matrix<double> paths(GetShortestPathsBetweenAllVertices(graph));
  Matrix<double> pheromone(mx_size, mx_size);
  for (int i{}; i < mx_size; i++) {
    for (int j{}; j < mx_size; j++) {
      pheromone(i, j) = 0.5 / mx_size;
    }
  }

  // initial walkthrough
  for (int i{}; i < mx_size * 6; i++) {
    TsmResult path = GenerateRandomPath(paths, &random_seed_);
    LeavingTrails(path, &pheromone);
    if (res.distance == 0 || path.distance < res.distance) res = path;
  }

  // repeating walkthroughs
  int silence{};
  while (silence < 5) {
    silence++;
    EvaporatePheromones(&pheromone);
    std::vector<TsmResult> path;
    path.resize(mx_size);
    for (int j{}; j < mx_size; j++) {
      path[j] = GeneratePopularPath(paths, &random_seed_, &pheromone);
      if (path[j].distance < res.distance) {
        res = path[j];
        silence = 0;
      }
    }
    for (int j{}; j < mx_size; j++) {
      LeavingTrails(path[j], &pheromone);
    }
  }
  res = InsertActualPaths(res, graph);
  return res;
}

TsmResult GraphAlgorithms::GenerateRandomPath(const Matrix<double> &paths,
                                              unsigned *seed) {
  TsmResult res{};
  int mx_size = paths.get_rows();
  for (int i{}; i < mx_size; i++) res.vertices.push_back(i);
  for (int i(1); i < mx_size - 1; i++) {
    std::swap(res.vertices[i], res.vertices[Roll(mx_size - i, seed) + i]);
  }
  res.vertices.push_back(0);

  auto iter = res.vertices.begin();
  while ((iter + 1) != res.vertices.end()) {
    res.distance += paths(*iter, *(iter + 1));
    iter++;
  }
  return res;
}

TsmResult GraphAlgorithms::GeneratePopularPath(const Matrix<double> &paths,
                                               unsigned *seed,
                                               Matrix<double> *phero) {
  TsmResult res{};
  res.vertices.push_back(0);
  int current_pos = 0;
  int mx_size = paths.get_rows();
  std::set<int> not_visited{};
  for (int i(1); i < mx_size; i++) not_visited.insert(i);

  while (not_visited.size()) {
    std::map<int, double> attractiveness{};
    double total{};
    for (auto i : not_visited) {
      double step_attr =
          (*phero)(current_pos, i) * (1.0 / paths(current_pos, i));
      attractiveness[i] = step_attr;
      total += step_attr;
    }
    double rnd = Roll(1000000, seed) / 1000000.0;
    for (auto i : not_visited) {
      rnd -= attractiveness[i];
      if (rnd < 0) {
        res.distance += paths(current_pos, i);
        current_pos = i;
        not_visited.erase(i);
        res.vertices.push_back(i);
        break;
      }
    }
  }
  res.distance += paths(current_pos, 0);
  res.vertices.push_back(0);
  return res;
}

void GraphAlgorithms::EvaporatePheromones(Matrix<double> *mx) {
  for (int i{}; i < mx->get_rows(); i++) {
    for (int j{}; j < mx->get_columns(); j++) {
      (*mx)(i, j) *= kEvaporationCoef;
    }
  }
}

void GraphAlgorithms::LeavingTrails(const TsmResult &path,
                                    Matrix<double> *phero) {
  auto iter = path.vertices.begin();
  while ((iter + 1) != path.vertices.end()) {
    (*phero)(*iter, *(iter + 1)) += 1.0 / path.distance;
    iter++;
  }
}

TsmResult GraphAlgorithms::InsertActualPaths(const TsmResult &data,
                                             const Graph &graph) {
  TsmResult res{};
  res.distance = data.distance;
  for (size_t i{}; i < data.vertices.size(); i++) {
    res.vertices.push_back(data.vertices[i] + 1);
    if (i < data.vertices.size() - 1) {
      if (graph.graphMatrix()(data.vertices[i], data.vertices[i + 1]) == 0) {
        std::vector<int> actual_path = GetVectorShortestPathBetweenVertices(
            graph, data.vertices[i] + 1, data.vertices[i + 1] + 1);
        for (size_t j{1}; j < actual_path.size() - 1; j++)
          res.vertices.push_back(actual_path[j]);
      }
    }
  }
  return res;
}

int GraphAlgorithms::Roll(int base, unsigned *seed) {
  int res{};
  if (base > 0) {
    res = rand_r(seed) % base;
  }
  return res;
}

// ========== 6: Alternative tsm algorithms ===========

TsmResult GraphAlgorithms::SolveTsmBrute(const Graph &graph) {
  TsmResult res{};
  Matrix<double> paths(GetShortestPathsBetweenAllVertices(graph));

  std::set<int> points{};
  for (int i(1); i < graph.MaxVertex(); i++) points.insert(i);
  res.vertices.push_back(0);

  res = RecBruteSearch(0, points, paths);

  res = InsertActualPaths(res, graph);
  return res;
}

TsmResult GraphAlgorithms::SolveTsmNearestNeighbour(const Graph &graph) {
  TsmResult res{};
  Matrix<double> paths(GetShortestPathsBetweenAllVertices(graph));

  std::set<int> points{};
  for (int i(1); i < graph.MaxVertex(); i++) points.insert(i);
  res.vertices.push_back(0);
  int curr_pos{};

  while (points.size() > 0) {
    double shortest{};
    int new_pos{};
    for (int i : points) {
      if (shortest == 0 || paths(curr_pos, i) < shortest) {
        shortest = paths(curr_pos, i);
        new_pos = i;
      }
    }
    res.vertices.push_back(new_pos);
    res.distance += paths(curr_pos, new_pos);
    points.erase(new_pos);
    curr_pos = new_pos;
  }
  res.vertices.push_back(0);
  res.distance += paths(curr_pos, 0);

  res = InsertActualPaths(res, graph);
  return res;
}

TsmResult GraphAlgorithms::RecBruteSearch(int cur, std::set<int> future,
                                          const Matrix<double> &paths) {
  TsmResult res{};
  if (future.size() == 0) {
    res.vertices.push_back(cur);
    res.vertices.push_back(0);
    res.distance = paths(cur, 0);
  } else {
    for (int i : future) {
      std::set<int> far_future(future);
      far_future.erase(i);
      TsmResult candidate = RecBruteSearch(i, far_future, paths);
      candidate.vertices.insert(candidate.vertices.begin(), cur);
      candidate.distance += paths(candidate.vertices[0], candidate.vertices[1]);
      if (res.distance == 0 || candidate.distance < res.distance) {
        res = candidate;
      }
    }
  }
  return res;
}

TsmResult GraphAlgorithms::SolveTsmAcoThreaded(const Graph &graph) {
  TsmResult res{};
  int mx_size = graph.MaxVertex();
  double initial_level(0.5 / mx_size);
  Matrix<double> paths(GetShortestPathsBetweenAllVertices(graph));
  Matrix<double> pheromone(mx_size, mx_size);
  for (int i{}; i < mx_size; i++)
    for (int j{}; j < mx_size; j++) pheromone(i, j) = initial_level;

  std::thread *th1{}, *th2{};

  // initial (random) walkthrough
  th1 = new std::thread(&GraphAlgorithms::AntInitRun, this, 3, paths,
                        &pheromone, rand_r(&random_seed_), &res);
  th2 = new std::thread(&GraphAlgorithms::AntInitRun, this, 3, paths,
                        &pheromone, rand_r(&random_seed_), &res);
  th1->join();
  th2->join();
  delete th1;
  delete th2;

  // repeating walkthrough
  int silence = 0;
  th1 = new std::thread(&GraphAlgorithms::AntRepeatRun, this, mx_size, paths,
                        &pheromone, rand_r(&random_seed_), &res, &silence);
  th2 = new std::thread(&GraphAlgorithms::AntRepeatRun, this, mx_size, paths,
                        &pheromone, rand_r(&random_seed_), &res, &silence);
  th1->join();
  th2->join();
  delete th1;
  delete th2;

  res = InsertActualPaths(res, graph);
  return res;
}

void GraphAlgorithms::AddPheromones(const Matrix<double> &source,
                                    Matrix<double> *target) {
  target->sum_matrix(source);
}

void GraphAlgorithms::AntInitRun(int rounds, const Matrix<double> paths,
                                 Matrix<double> *pheromone, unsigned seed,
                                 TsmResult *short_path) {
  Matrix<double> pheromone_change(paths.get_rows(), paths.get_columns());
  TsmResult best_path{};

  for (int i{}; i < rounds; i++) {
    TsmResult path = GenerateRandomPath(paths, &seed);
    if (best_path.distance == 0 || path.distance < best_path.distance)
      best_path = path;
    LeavingTrails(path, &pheromone_change);
  }

  mtx_.lock();
  AddPheromones(pheromone_change, pheromone);
  if (best_path.distance < short_path->distance || short_path->distance == 0)
    *short_path = best_path;
  mtx_.unlock();
}

void GraphAlgorithms::AntRepeatRun(int rounds, const Matrix<double> paths,
                                   Matrix<double> *pheromone, unsigned seed,
                                   TsmResult *short_path, int *silence) {
  mtx_.lock();
  Matrix<double> local_pheromone(*pheromone);
  mtx_.unlock();
  Matrix<double> pheromone_change(paths.get_rows(), paths.get_columns());
  TsmResult best_path{};
  bool flag(true);

  while (flag) {
    TsmResult path{};
    for (int i{}; i < rounds; i++) {
      path = GeneratePopularPath(paths, &seed, &local_pheromone);
      if (best_path.distance == 0 || path.distance < best_path.distance)
        best_path = path;
      LeavingTrails(path, &pheromone_change);
    }

    mtx_.lock();
    AddPheromones(pheromone_change, pheromone);
    EvaporatePheromones(pheromone);
    local_pheromone = *pheromone;
    if (best_path.distance < short_path->distance) {
      (*silence) = 0;
      *short_path = best_path;
    } else {
      (*silence)++;
      if (*silence >= 5) {
        flag = false;
      }
    }
    mtx_.unlock();
  }
}
