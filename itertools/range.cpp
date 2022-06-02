#pragma once
#include <iostream>

#define REVERSE_RANGE_IMPLEMENTED

class IterObject {

  struct Iterator {
    friend class IterObject;
    using Pointer = int64_t *;
    using Reference = int64_t &;

    Iterator(int64_t data, int64_t step) : data_(data), step_(step) {  // NOLINT
    }

    Reference operator*() {
      return data_;
    }
    Pointer operator->() {
      return &data_;
    }

    Iterator &operator++() {
      (data_) += step_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator temp = *this;
      (data_) += step_;
      return temp;
    }

    friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
      return lhs.data_ == rhs.data_;
    }

    friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
      return lhs.data_ != rhs.data_;
    }

   private:
    int64_t data_;
    int64_t step_;
  };

  struct ReverseIterator {
    using Pointer = int64_t *;
    using Reference = int64_t &;
    friend class IterObject;

    ReverseIterator(int64_t data, int64_t step) : data_(data), step_(step) {  // NOLINT
    }

    Reference operator*() {
      return data_;
    }
    Pointer operator->() {
      return &data_;
    }

    ReverseIterator &operator++() {
      (data_) -= step_;
      return *this;
    }

    ReverseIterator operator++(int) {
      ReverseIterator temp = *this;
      (data_) -= step_;
      return temp;
    }

    friend bool operator==(const ReverseIterator &lhs, const ReverseIterator &rhs) {
      return lhs.data_ == rhs.data_;
    }

    friend bool operator!=(const ReverseIterator &lhs, const ReverseIterator &rhs) {
      return lhs.data_ != rhs.data_;
    }

   private:
    int64_t data_;
    int64_t step_;
  };

 public:
  IterObject(const int64_t &begin, const int64_t &end, const int64_t &step) : begin_(begin), end_(end), step_(step) {
    if (step_ > 0) {
      if (end_ - begin_ >= 0) {
        size_ = (end_ - begin_) % step_ == 0 ? (end_ - begin_) / step_ : (end_ - begin_) / step_ + 1;
      } else {
        begin_ = 0;
        end_ = 0;
        size_ = 0;
      }
    } else if (step_ < 0) {
      if (end_ - begin_ <= 0) {
        size_ = std::abs(end_ - begin_) % std::abs(step_) == 0 ? std::abs(end_ - begin_) / std::abs(step_)
                                                               : std::abs(end_ - begin_) / std::abs(step_) + 1;
      } else {
        begin_ = 0;
        end_ = 0;
        size_ = 0;
      }
    } else {
      begin_ = 0;
      end_ = 0;
      size_ = 0;
    }
  }

  IterObject(const int64_t &end) : begin_(0), end_(end), step_(1) {  // NOLINT
    if (end >= 0) {
      size_ = end_;
    } else {
      begin_ = 0;
      end_ = 0;
      size_ = 0;
    }
  }
  const Iterator begin() const {  // NOLINT
    return {begin_, step_};
  }

  const Iterator end() const {  // NOLINT
    return {begin_ + step_ * size_, step_};
  }

  const ReverseIterator rbegin() const {  // NOLINT
    return ReverseIterator(begin_ + step_ * size_ - step_, step_);
  }
  const ReverseIterator rend() const {  // NOLINT
    return ReverseIterator(begin_ - step_, step_);
  }

 private:
  int64_t begin_;
  int64_t end_;
  int64_t step_;
  int64_t size_;
};

inline IterObject Range(const int &begin, const int &end, const int &step = 1) {
  return {begin, end, step};
}

inline IterObject Range(const int &end) {
  return {end};
}
