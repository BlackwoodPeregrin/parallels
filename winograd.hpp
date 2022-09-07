#ifndef SRC_WINOGRAD_HPP_
#define SRC_WINOGRAD_HPP_

#include "grape.hpp"
#include "mainwindow.hpp"

class WinogradWindow : public MainWindow {
 public:
  WinogradWindow();
  WinogradWindow(const WinogradWindow &window);
  WinogradWindow(WinogradWindow &&window);
  ~WinogradWindow();

  void operator=(const WinogradWindow &window);
  void operator=(WinogradWindow &&window);

 private:
  void ScanInput() override;
  void PrintUi() override;

  void SlotLoadFile();
  void SlotGenerate();
  void SlotSetRounds();
  void SlotSetWeight();
  void SlotStartTest();
  void PrintMatrix();

 private:
  bool weight_;
  GrapeAlgoritm *grape_;
};

#endif  // SRC_WINOGRAD_HPP_