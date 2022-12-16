#pragma once
#include <iostream>

template <class T>
class UniquePtr {
 public:
  UniquePtr() : ptr_(nullptr) {
  }
  explicit UniquePtr(T* a) : ptr_(a) {
  }
  UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
      other.ptr_ = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (this != &other) {
      delete ptr_;
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }

  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  T* Release() {
    T* temp_ptr = ptr_;
    ptr_ = nullptr;
    return temp_ptr;
  }

  void Reset(T* ptr = nullptr) {
    delete ptr_;
    ptr_ = ptr;
  }

  void Swap(UniquePtr<T>& other) {
    std::swap(other.ptr_, ptr_);
  }

  T* Get() const {
    return ptr_;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return (ptr_ != nullptr);
  }

  ~UniquePtr() {
    delete ptr_;
  }

 private:
  T* ptr_;
};