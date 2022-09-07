template <typename T>
s21::Stack<T>::Stack() : first_(nullptr), last_(nullptr), size_(0) {}

template <typename T>
s21::Stack<T>::Stack(std::initializer_list<T> const& items)
    : first_(nullptr), last_(nullptr), size_(0) {
  for (const T& value : items) {
    push(value);
  }
}

template <typename T>
s21::Stack<T>::Stack(const Stack& s)
    : first_(nullptr), last_(nullptr), size_(0) {
  *this = s;
}

template <typename T>
s21::Stack<T>::Stack(Stack&& s) : first_(nullptr), last_(nullptr), size_(0) {
  *this = std::move(s);
}

template <typename T>
s21::Stack<T>::~Stack() {
  clear();
}

template <typename T>
void s21::Stack<T>::clear() {
  while (last_ != nullptr) {
    pop();
  }
  first_ = nullptr;
}

template <typename T>
s21::Stack<T>& s21::Stack<T>::operator=(const Stack<T>& s) {
  if (this == &s) {
    throw std::invalid_argument("ERROR u can't move of object youself\n");
  }
  clear();
  Node* current = s.first_;
  while (current != nullptr) {
    push(current->data_);
    current = current->pNext_;
  }
  return *this;
}

template <typename T>
void s21::Stack<T>::operator=(Stack&& s) {
  if (this == &s) {
    throw std::invalid_argument("ERROR u can't move of object youself\n");
  }
  clear();
  swap(s);
}

template <typename T>
void s21::Stack<T>::push(const T& value) {
  if (last_ == nullptr) {
    last_ = new Node(value);
    first_ = last_;
  } else {
    if (first_ == last_) {
      last_ = new Node(value, nullptr, first_);
      first_->pNext_ = last_;
    } else {
      Node* newNode = new Node(value, nullptr, last_);
      last_->pNext_ = newNode;
      last_ = newNode;
    }
  }
  ++size_;
}

template <typename T>
T s21::Stack<T>::pop() {
  if (empty()) {
    throw std::out_of_range("ERROR in func pop(), stack is EMPTY");
  }
  Node* toDel = last_;
  T value = toDel->data_;
  last_ = last_->pPrev_;
  delete toDel;
  toDel = nullptr;
  --size_;
  return value;
}

template <typename T>
T s21::Stack<T>::peek() {
  if (empty()) {
    throw std::out_of_range("ERROR in func top(), stack is EMPTY");
  }
  return last_->data_;
}

template <typename T>
void s21::Stack<T>::swap(Stack& other) {
  if (this == &other) {
    throw std::invalid_argument(
        "ERROR in func swap(), can't swap yourself object\n");
  }
  std::swap(size_, other.size_);
  std::swap(last_, other.last_);
  std::swap(first_, other.first_);
}

template <typename T>
bool s21::Stack<T>::empty() {
  return (size_ == 0);
}

template <typename T>
const size_t& s21::Stack<T>::size() {
  return size_;
}
