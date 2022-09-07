#ifndef SRC_MAINWINDOW_HPP_
#define SRC_MAINWINDOW_HPP_

#include <string>

class MainWindow {
 public:
  MainWindow();
  MainWindow(const MainWindow &window);
  MainWindow(MainWindow &&window);
  ~MainWindow();

  void operator=(const MainWindow &window);
  void operator=(MainWindow &&window);

  void show();

 protected:
  virtual void ScanInput() {}
  virtual void PrintUi() {}
  void PauseFunc();
  std::string ProtectedInput();
  bool InputNumberCorrect(const std::string &number);
  std::string ReadNameFile();

 protected:
  bool m_exit_;
  bool m_draw_;
  unsigned exec_num_;
};

#endif  // SRC_MAINWINDOW_HPP_