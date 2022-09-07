#include "mainwindow.hpp"

#include <iostream>

MainWindow::MainWindow() : m_exit_(false), m_draw_(true), exec_num_(1) {}

MainWindow::MainWindow(const MainWindow &window)
    : m_exit_(false), m_draw_(true), exec_num_(1) {
  *this = window;
}

MainWindow::MainWindow(MainWindow &&window)
    : m_exit_(false), m_draw_(true), exec_num_(1) {
  *this = std::move(window);
}

MainWindow::~MainWindow() {}

void MainWindow::operator=(const MainWindow &window) {
  m_draw_ = window.m_draw_;
  exec_num_ = window.exec_num_;
}

void MainWindow::operator=(MainWindow &&window) {
  std::swap(m_draw_, window.m_draw_);
  std::swap(exec_num_, window.exec_num_);
}

void MainWindow::show() {
  while (m_exit_ == false) {
    if (m_draw_ == true) {
      PrintUi();
      m_draw_ = false;
    }
    ScanInput();
  }
}

void MainWindow::PauseFunc() {
  std::cout << "Enter any key to continue ... ";
  std::string key = ProtectedInput();
  m_draw_ = true;
}

bool MainWindow::InputNumberCorrect(const std::string &number) {
  size_t i = (number[0] == '-') ? 1 : 0;
  for (; i < number.size(); ++i) {
    if (number[i] < '0' || number[i] > '9') {
      return false;
    }
  }
  return true;
}

std::string MainWindow::ReadNameFile() {
  std::cout << "Input file name:" << std::endl;
  std::string file_name = ProtectedInput();
  return file_name;
}

std::string MainWindow::ProtectedInput() {
  std::string res{};
  std::cin >> res;
  if (std::cin.eof()) exit(0);
  return res;
}
