#pragma once
#include <utility>

#define WEAK_PTR_IMPLEMENTED

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

struct Count {
  int64_t strong_counter = 0;
  int64_t weak_counter = 0;
};

template <typename Y>
class WeakPtr;

template <typename T>
class SharedPtr {
 private:
  void CheckAndUpdate() {
    if (ptr_ != nullptr) {
      --counter_->strong_counter;
      if (!(counter_->strong_counter)) {
        delete ptr_;
        if (!(counter_->weak_counter)) {
          delete counter_;
        }
      }
    }
  }

 public:
  SharedPtr() : ptr_(nullptr), counter_(nullptr) {
  }
  SharedPtr(T *other) : ptr_(other) {  // NOLINT
    if (other != nullptr) {
      counter_ = new Count;
      counter_->strong_counter = 1;
    } else {
      counter_ = nullptr;
    }
  }
  SharedPtr(const SharedPtr<T> &other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (counter_ != nullptr) {
      ++(counter_->strong_counter);
    }
  }

  explicit SharedPtr(const WeakPtr<T> &other) {
    if (other.Expired()) {
      throw BadWeakPtr();
    }
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    ++(counter_->strong_counter);
  }

  SharedPtr &operator=(const SharedPtr<T> &other) {
    if (this != &other) {
      CheckAndUpdate();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      if (counter_ != nullptr) {
        ++counter_->strong_counter;
      }
    }
    return *this;
  }

  SharedPtr(SharedPtr<T> &&other) noexcept : ptr_(other.ptr_), counter_(other.counter_) {
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
  }

  SharedPtr &operator=(SharedPtr<T> &&other) noexcept {
    if (this != &other) {
      CheckAndUpdate();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      other.ptr_ = nullptr;
      other.counter_ = nullptr;
    }
    return *this;
  }

  void Reset(T *ptr = nullptr) {
    if (ptr_ != nullptr) {
      CheckAndUpdate();
    }
    ptr_ = ptr;
    if (ptr_ != nullptr) {
      counter_ = new Count;
      counter_->strong_counter = 1;
    } else {
      counter_ = nullptr;
    }
  }

  void Swap(SharedPtr<T> &other) {
    std::swap(other.ptr_, ptr_);
    std::swap(other.counter_, counter_);
  }

  T *Get() const {
    return ptr_;
  }

  int64_t UseCount() const {
    if (ptr_ == nullptr) {
      return 0;
    }
    return counter_->strong_counter;
  }

  T &operator*() const {
    return *ptr_;
  }

  T *operator->() const {
    return ptr_;
  }

  explicit operator bool() const noexcept {
    return (ptr_ != nullptr);
  }

  ~SharedPtr() {
    if (ptr_ != nullptr) {
      CheckAndUpdate();
      ptr_ = nullptr;
      counter_ = nullptr;
    }
  }
  friend class WeakPtr<T>;

 private:
  T *ptr_;
  Count *counter_;
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args &&... args) {
  return SharedPtr<T>{new T(std::forward<Args>(args)...)};
}

template <typename T>
class WeakPtr {
 public:
  friend class SharedPtr<T>;

  WeakPtr() : ptr_(nullptr), counter_(nullptr) {
  }
  WeakPtr(const WeakPtr<T> &other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (counter_ != nullptr) {
      ++counter_->weak_counter;
    }
  }
  WeakPtr(WeakPtr<T> &&other) : ptr_(other.ptr_), counter_(other.counter_) {
    other.counter_ = nullptr;
    other.ptr_ = nullptr;
  }

  WeakPtr &operator=(const WeakPtr<T> &other) {
    if (this != &other) {
      if (ptr_ != nullptr) {
        --counter_->weak_counter;
      }

      ptr_ = other.ptr_;
      counter_ = other.counter_;
      if (ptr_ != nullptr) {
        ++counter_->weak_counter;
      }
    }
    return *this;
  }
  WeakPtr &operator=(SharedPtr<T> &&other) noexcept {
    if (this != &other) {
      if (ptr_ != nullptr) {
        --counter_->weak_counter;
      }
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      other.ptr_ = nullptr;
      other.counter_ = nullptr;
    }
    return *this;
  }

  WeakPtr(const SharedPtr<T> &ptr) {  // NOLINT
    if (ptr.ptr_ != nullptr) {
      ptr_ = ptr.ptr_;
      counter_ = ptr.counter_;
      ++counter_->weak_counter;
    } else {
      ptr_ = nullptr;
      counter_ = nullptr;
    }
  }

  void Swap(WeakPtr<T> &lhs, WeakPtr<T> &rhs) {
    std::swap(lhs.counter_, rhs.counter_);
    std::swap(lhs.ptr_, rhs.ptr_);
  }

  void Reset() {
    if (ptr_ != nullptr) {
      --counter_->weak_counter;
    }
    ptr_ = nullptr;
    counter_ = nullptr;
  }

  int64_t UseCount() const {
    if (ptr_ == nullptr) {
      return 0;
    }
    return counter_->strong_counter;
  }

  bool Expired() const {
    if (counter_ == nullptr) {
      return true;
    }
    return !counter_->strong_counter;
  }

  SharedPtr<T> Lock() const {
    if (Expired()) {
      return nullptr;
    }
    return SharedPtr<T>(*this);
  }

  ~WeakPtr() {
    if (ptr_ != nullptr) {
      --counter_->weak_counter;
      if (!counter_->strong_counter) {
        if (!counter_->weak_counter) {
          delete counter_;
        }
        counter_ = nullptr;
      }
    }
  }

 private:
  T *ptr_;
  Count *counter_;
};
