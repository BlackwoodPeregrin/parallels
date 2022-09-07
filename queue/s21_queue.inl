template <typename T>
s21::Queue<T>::Queue() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
s21::Queue<T>::Queue(std::initializer_list<T> const& items)
    : head_(nullptr), tail_(nullptr), size_(0) {
  for (const T& value : items) {
    push(value);
  }
}

template <typename T>
s21::Queue<T>::Queue(const Queue& q)
    : head_(nullptr), tail_(nullptr), size_(0) {
  *this = q;
}

template <typename T>
s21::Queue<T>::Queue(Queue&& q) : head_(nullptr), tail_(nullptr), size_(0) {
  *this = std::move(q);
}

template <typename T>
void s21::Queue<T>::clear() {
  while (head_ != nullptr) {
    pop();
  }
  tail_ = nullptr;
  size_ = 0;
}

template <typename T>
s21::Queue<T>::~Queue() {
  clear();
}

template <typename T>
s21::Queue<T>& s21::Queue<T>::operator=(const Queue& q) {
  if (this == &q) {
    throw std::invalid_argument("ERROR u can't copy of object youself\n");
  }
  clear();
  Node* current = q.head_;
  while (current != nullptr) {
    push(current->data_);
    current = current->pNext_;
  }
  return *this;
}

template <typename T>
void s21::Queue<T>::operator=(Queue&& q) {
  clear();
  swap(q);
}

template <typename T>
const T& s21::Queue<T>::front() {
  if (empty()) {
    throw std::out_of_range("Error in func front(), list is EMPTY\n");
  }
  return head_->data_;
}

template <typename T>
const T& s21::Queue<T>::back() {
  if (empty()) {
    throw std::out_of_range("Error in func back(), list is EMPTY\n");
  }
  return tail_->data_;
}

template <typename T>
bool s21::Queue<T>::empty() {
  return (size_ == 0);
}

template <typename T>
const size_t& s21::Queue<T>::size() {
  return size_;
}

template <typename T>
void s21::Queue<T>::push(const T& value) {
  if (head_ == nullptr) {
    head_ = new Node(value);
    tail_ = head_;
  } else {
    if (head_ == tail_) {
      tail_ = new Node(value);
      head_->pNext_ = tail_;
    } else {
      Node* newElement = new Node(value);
      tail_->pNext_ = newElement;
      tail_ = newElement;
    }
  }
  ++size_;
}

template <typename T>
T s21::Queue<T>::pop() {
  if (empty()) {
    throw std::out_of_range("ERROR in func pop(), queue is EMPTY");
  }
  Node* toDel = head_;
  T value = toDel->data_;
  head_ = head_->pNext_;
  delete toDel;
  toDel = nullptr;
  --size_;
  return value;
}

template <typename T>
void s21::Queue<T>::swap(Queue& q) {
  if (this == &q) {
    throw std::invalid_argument(
        "ERROR in func swap(), can't swap yourself object\n");
  }
  std::swap(head_, q.head_);
  std::swap(tail_, q.tail_);
  std::swap(size_, q.size_);
}
