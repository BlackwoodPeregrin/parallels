#include "winograd.hpp"

#include <chrono>

int main() {
  WinogradWindow window;
  window.show();
}

WinogradWindow::WinogradWindow() : weight_(false), grape_(new GrapeAlgoritm) {}

WinogradWindow::WinogradWindow(const WinogradWindow &window)
    : MainWindow(), weight_(false), grape_(new GrapeAlgoritm) {
  *this = window;
}

WinogradWindow::WinogradWindow(WinogradWindow &&window)
    : MainWindow(), weight_(false), grape_(new GrapeAlgoritm) {
  *this = std::move(window);
}

WinogradWindow::~WinogradWindow() { delete grape_; }

void WinogradWindow::operator=(const WinogradWindow &window) {
  m_draw_ = window.m_draw_;
  exec_num_ = window.exec_num_;
  weight_ = window.weight_;
  *grape_ = *(window.grape_);
}

void WinogradWindow::operator=(WinogradWindow &&window) {
  std::swap(m_draw_, window.m_draw_);
  std::swap(exec_num_, window.exec_num_);
  std::swap(grape_, window.grape_);
  std::swap(weight_, window.weight_);
}

void WinogradWindow::ScanInput() {
  std::string choice = ProtectedInput();
  if (choice == "1") {
    SlotLoadFile();
  } else if (choice == "2") {
    SlotGenerate();
  } else if (choice == "3") {
    SlotSetRounds();
  } else if (choice == "4") {
    SlotSetWeight();
  } else if (choice == "5") {
    SlotStartTest();
  } else if (choice == "6") {
    std::cout << "bye!" << std::endl;
    m_exit_ = true;
  } else {
    std::cout << "Incorrect choice, try again." << std::endl;
  }
}

void WinogradWindow::PrintUi() {
  std::cout.flush();
  std::cin.clear();
  system("clear");
  std::cout << " === Winograd algorithm ===\n";
  std::cout << "matrices loaded:\t";
  std::cout << (grape_->IsMatrixLoaded(1) ? "true" : "false") << "\t";
  std::cout << (grape_->IsMatrixLoaded(2) ? "true" : "false") << "\n";
  std::cout << "matrices sizes :\t";
  if (grape_->IsMatrixLoaded(1))
    std::cout << grape_->MatricesSizes(1).first << "*"
              << grape_->MatricesSizes(1).second << "\t";
  else
    std::cout << "n/a\t";
  if (grape_->IsMatrixLoaded(2))
    std::cout << grape_->MatricesSizes(2).first << "*"
              << grape_->MatricesSizes(2).second << "\n";
  else
    std::cout << "n/a\n";
  std::cout << "number of executions:\t" << exec_num_ << "\n";
  std::cout << "additional delay:\t" << (weight_ ? "true" : "false") << "\n\n";
  std::cout << "1. Load matrix from a file\n";
  std::cout << "2. Generate random matrices\n";
  std::cout << "3. Set number of executions\n";
  std::cout << "4. Switch additional delay\n";
  std::cout << "5. Start test\n";
  std::cout << "6. Exit" << std::endl;
}

void WinogradWindow::SlotLoadFile() {
  std::cout << "Choose a matrix (1 or 2):" << std::endl;
  std::string line = ProtectedInput();
  bool num{}, correct(true);
  if (line == "1")
    num = false;
  else if (line == "2")
    num = true;
  else
    correct = false;

  if (correct) {
    line = ReadNameFile();
    try {
      grape_->LoadMatrixFromFile(line, num);
      std::cout << "matrix loaded" << std::endl;
    } catch (const std::exception &e) {
      std::cout << e.what() << std::endl;
    }
  } else {
    std::cout << "incorrect index" << std::endl;
  }
  PauseFunc();
}

void WinogradWindow::SlotGenerate() {
  int sizes[3];
  bool correct(true);
  std::cout << "Input three sizes:" << std::endl;
  std::string line;
  for (int i{}; i < 3 && correct; i++) {
    line = ProtectedInput();
    if (InputNumberCorrect(line) == true) {
      try {
        sizes[i] = std::stoi(line);
        if (sizes[i] < 1) correct = false;
      } catch (const std::exception &e) {
        correct = false;
      }
    } else {
      correct = false;
    }
  }

  if (correct) {
    grape_->GenerateRandomMatrix(std::make_pair(sizes[0], sizes[1]),
                                 std::make_pair(sizes[1], sizes[2]));
  } else {
    std::cout << "Incorrect input" << std::endl;
  }
  PauseFunc();
}

void WinogradWindow::SlotSetRounds() {
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

void WinogradWindow::SlotSetWeight() {
  weight_ = !weight_;
  m_draw_ = true;
}

void WinogradWindow::SlotStartTest() {
  if (!grape_->IsMatrixLoaded(1) || !grape_->IsMatrixLoaded(2)) {
    std::cout << "you must load two matrices first!" << std::endl;
  } else if (grape_->MatricesSizes(1).second !=
             grape_->MatricesSizes(2).first) {
    std::cout << "incorrect matrices sizes!" << std::endl;
  } else {
    std::cout << " === Running Tests ===" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned i{}; i < exec_num_; i++) {
      grape_->CalcSingleThread(weight_);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double duration =
        (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
            .count() /
        1000.0;
    std::cout << "\nsingle thread" << std::endl;
    std::cout << "time:\t\t" << duration << " s" << std::endl;
    std::cout << "result:\n";
    PrintMatrix();

    start = std::chrono::high_resolution_clock::now();
    for (unsigned i{}; i < exec_num_; i++) {
      grape_->CalcMultiThreadClassic(weight_);
    }
    end = std::chrono::high_resolution_clock::now();
    duration =
        (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
            .count() /
        1000.0;
    std::cout << "\nclassic multithread" << std::endl;
    std::cout << "time:\t\t" << duration << " s" << std::endl;
    std::cout << "result:\n";
    PrintMatrix();

    start = std::chrono::high_resolution_clock::now();
    for (unsigned i{}; i < exec_num_; i++) {
      grape_->CalcMultiThreadConveyor(weight_);
    }
    end = std::chrono::high_resolution_clock::now();
    duration =
        (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
            .count() /
        1000.0;
    std::cout << "\npipeline multithread" << std::endl;
    std::cout << "time:\t\t" << duration << " s" << std::endl;
    std::cout << "result:\n";
    PrintMatrix();
  }

  PauseFunc();
}

void WinogradWindow::PrintMatrix() {
  Matrix<float> answer = grape_->GetResultMatrix();
  for (int i{}; i < answer.get_rows(); i++) {
    for (int j{}; j < answer.get_columns(); j++) {
      std::cout << answer(i, j) << " ";
    }
    std::cout << std::endl;
  }
}
