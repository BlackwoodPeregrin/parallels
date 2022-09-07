#pragma once

#include <iostream>

namespace s21 {
template <typename T>
class Queue {
 private:
  class Node {
   public:
    Node *pNext_;
    T data_;
    explicit Node(const T &data = T(), Node *pNext = nullptr)
        : pNext_(pNext), data_(data) {}
  };
  Node *head_;
  Node *tail_;
  size_t size_;

 public:
  Queue();
  explicit Queue(std::initializer_list<T> const &items);
  explicit Queue(const Queue &q);
  explicit Queue(Queue &&q);
  ~Queue();
  void operator=(Queue &&q);
  Queue &operator=(const Queue &q);

  const T &front();
  const T &back();

  bool empty();
  const size_t &size();

  void push(const T &value);
  T pop();
  T peek();
  void swap(Queue &other);
  void clear();
};
}  // namespace s21

#include "s21_queue.inl"
