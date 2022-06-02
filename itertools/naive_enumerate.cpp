#pragma once

template <typename Container>
class Enumerate {
 public:
  class Iterator {
   public:
    Iterator(size_t pos, typename Container::iterator &val) {
      pos_ = pos;
      ptr_ = val;
    }

    auto operator*() {
      return std::pair<size_t, typename Container::reference>(pos_, *ptr_);
    }

    Iterator &operator++() {
      ++pos_;
      ++ptr_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++pos_;
      ++ptr_;
      return tmp;
    }

    friend bool operator==(Iterator &first, Iterator &second) {
      return first.pos_ == second.pos_;
    }

    friend bool operator!=(Iterator &first, Iterator &second) {
      return first.pos_ != second.pos_;
    }

    Iterator &operator=(const Iterator &other) {
      pos_ = other.pos_;
      ptr_ = other.ptr_;
    }

   private:
    size_t pos_;
    typename Container::iterator ptr_;
  };

  class ConstIterator {
   public:
    ConstIterator(size_t pos, const typename Container::const_iterator &val) : pos_(pos), data_(val) {
    }

    std::pair<size_t, typename Container::const_reference> operator*() {
      return {pos_, *data_};
    }

    ConstIterator &operator++() {
      ++pos_;
      ++data_;
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator tmp = *this;
      ++pos_;
      ++data_;
      return tmp;
    }

    friend bool operator==(ConstIterator &first, ConstIterator &second) {
      return first.pos_ == second.pos_;
    }

    friend bool operator!=(ConstIterator &first, ConstIterator &second) {
      return first.pos_ != second.pos_;
    }

    Iterator &operator=(const ConstIterator &other) {
      pos_ = other.pos_;
      data_ = other.data_;
    }

   private:
    size_t pos_;
    typename Container::const_iterator data_;
  };

  explicit Enumerate(Container &container) : begin_(container.begin()), end_(container.end()) {
    size_ = 0;
    for (auto i = std::begin(container); i != std::end(container); ++i) {
      ++size_;
    }
  }

  explicit Enumerate(const Container &container) : cbegin_(container.begin()), cend_(container.end()) {
    size_ = 0;
    for (auto i = std::begin(container); i != std::end(container); ++i) {
      ++size_;
    }
  }

  Iterator begin() {  // NOLINT
    return Iterator(0, begin_);
  }

  Iterator end() {  // NOLINT
    return Iterator(size_, end_);
  }

  ConstIterator begin() const {  // NOLINT
    return ConstIterator(0, cbegin_);
  }

  ConstIterator end() const {  // NOLINT
    return ConstIterator(size_, cend_);
  }

 private:
  typename Container::iterator begin_;
  typename Container::iterator end_;
  typename Container::const_iterator cbegin_;
  typename Container::const_iterator cend_;
  size_t size_;
};
