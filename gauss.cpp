#include "gauss.hpp"

#include <chrono>

int main() {
  GaussWindow window;
  window.show();
}

GaussWindow::GaussWindow() : weight_(false), calc_slough_(new CalcSlough) {}

GaussWindow::GaussWindow(const GaussWindow &window)
    : MainWindow(), weight_(false), calc_slough_(new CalcSlough) {
  *this = window;
}

GaussWindow::GaussWindow(GaussWindow &&window)
    : MainWindow(), weight_(false), calc_slough_(new CalcSlough) {
  *this = std::move(window);
}

GaussWindow::~GaussWindow() { delete calc_slough_; }

void GaussWindow::operator=(const GaussWindow &window) {
  m_draw_ = window.m_draw_;
  exec_num_ = window.exec_num_;
  *calc_slough_ = *(window.calc_slough_);
  weight_ = window.weight_;
}

void GaussWindow::operator=(GaussWindow &&window) {
  std::swap(m_draw_, window.m_draw_);
  std::swap(exec_num_, window.exec_num_);
  std::swap(calc_slough_, window.calc_slough_);
  std::swap(weight_, window.weight_);
}

void GaussWindow::ScanInput() {
  std::string choice = ProtectedInput();
  if (choice == "1") {
    SlotLoadAFile();
  } else if (choice == "2") {
    SlotSetRounds();
  } else if (choice == "3") {
    SlotSetWeight();
  } else if (choice == "4") {
    SlotStartTest();
  } else if (choice == "5") {
    std::cout << "bye!" << std::endl;
    m_exit_ = true;
  } else {
    std::cout << "Incorrect choice, try again." << std::endl;
  }
}

void GaussWindow::PrintUi() {
  std::cout.flush();
  std::cin.clear();
  system("clear");
  std::cout << " === Solving systems of linear equations ===\n";
  std::cout << "SLE loaded:\t\t"
            << (calc_slough_->IsMatrixLoaded() ? "true" : "false") << "\n";
  std::cout << "number of executions:\t" << exec_num_ << "\n";
  std::cout << "additional delay:\t" << (weight_ ? "true" : "false") << "\n\n";
  std::cout << "1. Load SLE from a file\n";
  std::cout << "2. Set number of executions\n";
  std::cout << "3. Switch additional delay\n";
  std::cout << "4. Start test\n";
  std::cout << "5. Exit" << std::endl;
}

void GaussWindow::SlotLoadAFile() {
  std::string file_name = ReadNameFile();
  try {
    calc_slough_->LoadSloughFromFile(file_name);
    std::cout << "SLE loaded" << std::endl;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  PauseFunc();
}

void GaussWindow::SlotSetRounds() {
  int num;
  bool correct(true);
  std::cout << "Input number:" << std::endl;
  std::string line = ProtectedInput();
  if (InputNumberCorrect(line) == true) {
    try {
      num = std::stoi(line);
    } catch (const std::exception &e) {
      correct = false;
    }
  } else {
    correct = false;
  }

  if (correct && num > 0) {
    exec_num_ = num;
  } else {
    std::cout << "Incorrect input" << std::endl;
  }
  PauseFunc();
}

void GaussWindow::SlotSetWeight() {
  weight_ = !weight_;
  m_draw_ = true;
}

void GaussWindow::SlotStartTest() {
  if (!calc_slough_->IsMatrixLoaded()) {
    std::cout << "you must load a matrix first!" << std::endl;
  } else {
    CalcSlough A;
    std::cout << " === Running Tests ===" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned i{}; i < exec_num_; i++) {
      A = *calc_slough_;
      A.CalcSloughtSingleThread(weight_);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double duration =
        (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
            .count() /
        1000.0;
    std::cout << "\nsingle threaded" << std::endl;
    std::cout << "time:\t\t" << duration << " s" << std::endl;
    std::cout << "result:\t";
    auto res = A.GetResultSlough();
    for (float &iter : res) std::cout << iter << " ";
    std::cout << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (unsigned i{}; i < exec_num_; i++) {
      A = *calc_slough_;
      A.CalcSloughtMultiThread(weight_);
    }
    end = std::chrono::high_resolution_clock::now();
    duration =
        (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
            .count() /
        1000.0;
    std::cout << "\nmulti threaded" << std::endl;
    std::cout << "time:\t\t" << duration << " s" << std::endl;
    std::cout << "result:\t";
    res = A.GetResultSlough();
    for (float &iter : res) std::cout << iter << " ";
    std::cout << std::endl;
  }

  PauseFunc();
}
