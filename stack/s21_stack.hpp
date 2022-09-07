#pragma once

#include <iostream>

namespace s21 {
template <typename T>
class Stack {
 private:
  class Node {
   public:
    Node *pNext_;
    Node *pPrev_;
    T data_;
    explicit Node(const T &data = T(), Node *pNext = nullptr,
                  Node *pPrev = nullptr)
        : pNext_(pNext), pPrev_(pPrev), data_(data) {}
  };
  Node *first_;
  Node *last_;
  size_t size_;

 public:
  Stack();
  explicit Stack(std::initializer_list<T> const &items);
  explicit Stack(const Stack &s);
  explicit Stack(Stack &&s);
  ~Stack();
  void clear();

  void operator=(Stack<T> &&s);
  Stack &operator=(const Stack<T> &s);

  void push(const T &value);
  T pop();
  T peek();
  void swap(Stack &other);
  bool empty();
  const size_t &size();
};
}  // namespace s21

#include "s21_stack.inl"
