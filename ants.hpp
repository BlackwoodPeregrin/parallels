#ifndef SRC_ANTS_HPP_
#define SRC_ANTS_HPP_

#include "mainwindow.hpp"
#include "s21_graph_algorithms.hpp"

class AntsWindow : public MainWindow {
 public:
  AntsWindow();
  AntsWindow(const AntsWindow &window);
  AntsWindow(AntsWindow &&window);
  ~AntsWindow();

  void operator=(const AntsWindow &window);
  void operator=(AntsWindow &&window);

 private:
  void ScanInput() override;
  void PrintUi() override;

  void SlotOne();
  void SlotTwo();
  void SlotThree();
  void SlotFour();

 private:
  Graph *graph_;
  GraphAlgorithms graph_alg_;
};

#endif  // SRC_ANTS_HPP_