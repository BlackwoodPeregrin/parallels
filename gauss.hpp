#ifndef SRC_GAUSS_HPP_
#define SRC_GAUSS_HPP_

#include "mainwindow.hpp"
#include "slough.hpp"

class GaussWindow : public MainWindow {
 public:
  GaussWindow();
  GaussWindow(const GaussWindow &window);
  GaussWindow(GaussWindow &&window);
  ~GaussWindow();

  void operator=(const GaussWindow &window);
  void operator=(GaussWindow &&window);

 private:
  void ScanInput() override;
  void PrintUi() override;

  void SlotLoadAFile();
  void SlotSetRounds();
  void SlotSetWeight();
  void SlotStartTest();

 private:
  bool weight_;
  CalcSlough *calc_slough_;
};

#endif  // SRC_GAUSS_HPP_