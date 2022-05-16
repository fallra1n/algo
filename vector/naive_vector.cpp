#pragma once
#include <stdexcept>
#include <iostream>

template <typename T>
class Vector {
 private:
  void ToEmpty() {
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }
  void Redistribute(T *other) {
    std::move(buffer_, buffer_ + size_, other);
    delete[] buffer_;
    buffer_ = other;
  }

 public:
  using ValueType = T;
  using Pointer = T *;
  using ConstPointer = const T *;
  using Reference = T &;
  using ConstReference = const T &;
  using SizeType = size_t;
  using Iterator = T *;
  using iterator_ = T *;  // NOLINT
  using ConstIterator = const T *;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() : size_(0), capacity_(0), buffer_(nullptr) {
  }
  explicit Vector(const SizeType &size) : size_(size), capacity_(size), buffer_(new ValueType[size]) {
  }

  Vector(int size, const ValueType &value) {
    if (size > 0) {
      buffer_ = new ValueType[size];
      size_ = 0;
      try {
        for (; size_ < static_cast<size_t>(size); ++size_) {
          buffer_[size_] = value;
        }
      } catch (...) {
        delete[] buffer_;
        ToEmpty();
      }
      capacity_ = static_cast<SizeType>(size);
    } else {
      buffer_ = nullptr;
      ToEmpty();
    }
  }

  Vector(const std::initializer_list<ValueType> &init) {
    buffer_ = new ValueType[static_cast<SizeType>(init.end() - init.begin())];
    capacity_ = static_cast<SizeType>(init.end() - init.begin());
    size_ = static_cast<SizeType>(init.end() - init.begin());
    try {
      std::copy(init.begin(), init.end(), buffer_);
    } catch (...) {
      delete[] buffer_;
      ToEmpty();
    }
  }
  Vector(const Vector &other) {
    if (other.size_ > 0) {
      buffer_ = new ValueType[other.size_];
      capacity_ = other.size_;
      size_ = other.size_;
      try {
        std::copy(other.Data(), other.Data() + size_, buffer_);
      } catch (...) {
        delete[] buffer_;
        ToEmpty();
      }
    } else {
      ToEmpty();
    }
  }
  Vector(Vector &&other) noexcept {
    buffer_ = other.buffer_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.ToEmpty();
  }

  template <typename InputIt>
  Vector(const InputIt &first, const InputIt &last) {
    size_ = static_cast<SizeType>(last - first);
    capacity_ = static_cast<SizeType>(last - first);
    if (size_ > 0) {
      buffer_ = new ValueType[capacity_];
      try {
        std::copy(first, last, buffer_);
      } catch (...) {
        delete[] buffer_;
        ToEmpty();
      }
    } else {
      buffer_ = nullptr;
    }
  }

  Vector &operator=(const Vector &other) {
    if (this != &other) {
      Vector<T>(other).Swap(*this);
    }
    return *this;
  }
  Vector &operator=(Vector &&other) noexcept {
    Vector<ValueType>(std::move(other)).Swap(*this);
    return *this;
  }
  Vector &operator=(const std::initializer_list<ValueType> &init) {
    delete[] buffer_;
    if (static_cast<SizeType>(init.end() - init.begin()) > 0) {
      buffer_ = new ValueType[static_cast<SizeType>(init.end() - init.begin())];
      capacity_ = static_cast<SizeType>(init.end() - init.begin());
      size_ = static_cast<SizeType>(init.end() - init.begin());
      std::move(init.begin(), init.end(), buffer_);
    } else {
      ToEmpty();
    }
    return *this;
  }

  SizeType Capacity() const {
    return capacity_;
  }
  SizeType Size() const {
    return size_;
  }
  bool Empty() const {
    return (size_ == 0);
  }

  Reference operator[](SizeType pos) {
    return buffer_[pos];
  }
  ConstReference operator[](SizeType pos) const {
    return buffer_[pos];
  }

  Reference At(SizeType pos) {
    if (pos > size_ - 1) {
      throw std::out_of_range("std::out_of_range");
    }
    return buffer_[pos];
  }
  ConstReference At(SizeType pos) const {
    if (pos > size_ - 1) {
      throw std::out_of_range("std::out_of_range");
    }
    return buffer_[pos];
  }

  Reference Back() {
    return buffer_[size_ - 1];
  };
  ConstReference Back() const {
    return buffer_[size_ - 1];
  };

  Reference Front() {
    return buffer_[0];
  }
  ConstReference Front() const {
    return buffer_[0];
  }

  Pointer Data() {
    return buffer_;
  }

  ConstPointer Data() const {
    return buffer_;
  }

  void Swap(Vector<ValueType> &other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  void Resize(SizeType new_size) {
    if (new_size > capacity_) {
      auto temp = new ValueType[new_size];
      Redistribute(temp);
      capacity_ = new_size;
    }
    size_ = new_size;
  }
  void Resize(SizeType new_size, const ValueType &value) {
    size_t mind = size_;
    try {
      size_t old = size_;
      Resize(new_size);
      for (; old < new_size; ++old) {
        buffer_[old] = value;
      }
    } catch (...) {
      size_ = mind;
    }
  }

  void Reserve(SizeType new_cap) {
    if (new_cap > capacity_) {
      auto temp = new ValueType[new_cap];
      Redistribute(temp);
      capacity_ = new_cap;
    }
  }

  void ShrinkToFit() {
    if (size_ > 0) {
      auto temp = new ValueType[size_];
      Redistribute(temp);
      capacity_ = size_;
    } else {
      delete[] buffer_;
      ToEmpty();
    }
  }

  void Clear() {
    size_ = 0;
  }

  void PushBack(const ValueType &value) {
    if (size_ + 1 > capacity_) {
      SizeType new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      Reserve(new_cap);
    }
    size_t old = size_;
    try {
      buffer_[size_++] = std::move(value);
    } catch (...) {
      size_ = old;
    }
  }
  void PushBack(ValueType &&value) {
    if (size_ + 1 > capacity_) {
      SizeType new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
      Reserve(new_cap);
    }
    buffer_[size_++] = std::move(value);
  }

  void PopBack() {
    --size_;
  }

  Iterator begin() {  // NOLINT
    return iterator_(buffer_);
  }
  Iterator end() {  // NOLINT
    return iterator_(buffer_ + size_);
  }

  ConstIterator begin() const {  // NOLINT
    return ConstIterator(buffer_);
  }
  ConstIterator end() const {  // NOLINT
    return ConstIterator(buffer_ + size_);
  }

  ConstIterator cbegin() const {  // NOLINT
    return begin();
  }
  ConstIterator cend() const {  // NOLINT
    return end();
  }

  ReverseIterator rbegin() {  // NOLINT
    return std::make_reverse_iterator(buffer_ + size_);
  }
  ReverseIterator rend() {  // NOLINT
    return std::make_reverse_iterator(buffer_);
  }

  ConstReverseIterator rbegin() const {  // NOLINT
    return std::make_reverse_iterator(Iterator(buffer_ + size_));
  }
  ConstReverseIterator rend() const {  // NOLINT
    return std::make_reverse_iterator(Iterator(buffer_));
  }

  ConstReverseIterator crbegin() {  // NOLINT
    return std::make_reverse_iterator(Iterator(buffer_ + size_));
  }
  ConstReverseIterator crend() {  // NOLINT
    return std::make_reverse_iterator(Iterator(buffer_));
  }

  ConstReverseIterator crbegin() const {  // NOLINT
    return std::make_reverse_iterator(Iterator(buffer_ + size_));
  }
  ConstReverseIterator crend() const {  // NOLINT
    return std::make_reverse_iterator(Iterator(buffer_));
  }

  ~Vector() {
    delete[] buffer_;
  }

 private:
  SizeType size_;
  SizeType capacity_;
  ValueType *buffer_;
};

template <typename T>
bool operator<(const Vector<T> &first, const Vector<T> &second) {
  size_t i = 0;
  while (i < first.Size() && i < second.Size()) {
    if (first[i] != second[i]) {
      return static_cast<size_t>(first[i]) < static_cast<size_t>(second[i]);
    }
    ++i;
  }
  return (first.Size() < second.Size());
}
template <typename T>
bool operator==(const Vector<T> &first, const Vector<T> &second) {
  return !(first < second) && !(second < first);
}
template <typename T>
bool operator!=(const Vector<T> &first, const Vector<T> &second) {
  return ((first < second) || (second < first));
}
template <typename T>
bool operator>(const Vector<T> &first, const Vector<T> &second) {
  return (second < first);
}
template <typename T>
bool operator<=(const Vector<T> &first, const Vector<T> &second) {
  return (!(second < first));
}
template <typename T>
bool operator>=(const Vector<T> &first, const Vector<T> &second) {
  return (!(first < second));
}
