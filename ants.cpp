#include "ants.hpp"

int main() {
  AntsWindow window;
  window.show();
}

AntsWindow::AntsWindow() : graph_(new Graph) {}

AntsWindow::AntsWindow(const AntsWindow &window)
    : MainWindow(), graph_(new Graph) {
  *this = window;
}

AntsWindow::AntsWindow(AntsWindow &&window) : MainWindow(), graph_(new Graph) {
  *this = std::move(window);
}

AntsWindow::~AntsWindow() { delete graph_; }

void AntsWindow::operator=(const AntsWindow &window) {
  m_draw_ = window.m_draw_;
  exec_num_ = window.exec_num_;
  *graph_ = *(window.graph_);
}

void AntsWindow::operator=(AntsWindow &&window) {
  std::swap(m_draw_, window.m_draw_);
  std::swap(exec_num_, window.exec_num_);
  std::swap(graph_, window.graph_);
}

void AntsWindow::ScanInput() {
  std::string choice = ProtectedInput();
  if (choice == "1") {
    SlotOne();
  } else if (choice == "2") {
    SlotTwo();
  } else if (choice == "3") {
    SlotThree();
  } else if (choice == "4") {
    SlotFour();
  } else if (choice == "5") {
    std::cout << "bye!" << std::endl;
    m_exit_ = true;
  } else {
    std::cout << "Incorrect choice, try again." << std::endl;
  }
}

void AntsWindow::PrintUi() {
  std::cout.flush();
  std::cin.clear();
  system("clear");
  std::cout << " === Ants Colony Optimization algorithm ===\n";
  std::cout << "graph loaded:\t\t"
            << (graph_->IsGraphInizialize() ? "true" : "false") << "\n";
  std::cout << "graph size:\t\t";
  if (graph_->IsGraphInizialize())
    std::cout << graph_->MaxVertex();
  else
    std::cout << "n/a";
  std::cout << "\n";
  std::cout << "number of executions:\t" << exec_num_ << "\n\n";
  std::cout << "1. Load graph from a file\n";
  std::cout << "2. Generate random graph\n";
  std::cout << "3. Set number of executions\n";
  std::cout << "4. Start test\n";
  std::cout << "5. Exit" << std::endl;
}

void AntsWindow::SlotOne() {
  std::string file_name = ReadNameFile();
  try {
    graph_->LoadGraphFromFile(file_name);
    std::cout << "graph loaded." << std::endl;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  PauseFunc();
}

void AntsWindow::SlotTwo() {
  int num;
  bool correct(true);
  std::cout << "Input size:" << std::endl;
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
    graph_->GenerateUnorientedGraph(num, true);
  } else {
    std::cout << "Incorrect input" << std::endl;
  }
  PauseFunc();
}

void AntsWindow::SlotThree() {
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

void AntsWindow::SlotFour() {
  try {
    TsmResult res{};
    double sum_distance{};
    std::cout << " === Running Tests ===" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned i{}; i < exec_num_; i++) {
      res = graph_alg_.SolveTravelingSalesmanProblem(*graph_);
      sum_distance += res.distance;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double duration =
        (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
            .count() /
        1000.0;
    std::cout << "\nsingle threaded" << std::endl;
    std::cout << "avg distance:\t" << sum_distance / exec_num_ << std::endl;
    std::cout << "time:\t\t" << duration << " s" << std::endl;
    sum_distance = 0;

    start = std::chrono::high_resolution_clock::now();
    for (unsigned i{}; i < exec_num_; i++) {
      res = graph_alg_.SolveTsmAcoThreaded(*graph_);
      sum_distance += res.distance;
    }
    end = std::chrono::high_resolution_clock::now();
    duration =
        (std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
            .count() /
        1000.0;
    std::cout << "\nmulti threaded" << std::endl;
    std::cout << "avg distance:\t" << sum_distance / exec_num_ << std::endl;
    std::cout << "time:\t\t" << duration << " s" << std::endl;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  PauseFunc();
}
