#pragma once
#include <utility>
#include <iterator>

template <class Iterator, class Container>
class EnumerateProxy {
  Iterator begin_;
  Iterator end_;

  Container container_;

 public:


  explicit EnumerateProxy(Iterator begin, Iterator end) : begin_(begin), end_(end) {
  }

  explicit EnumerateProxy(Container container) : container_(container) {
    begin_ = container_.begin();
    end_ = container_.end();
  }

  struct ClassIterator {
   private:
    Iterator iter_;
    size_t pos_;

   public:
    using reference = typename Iterator::reference;

    explicit ClassIterator(Iterator iter, size_t pos = 0) : iter_(iter), pos_(pos) {
    }
    ClassIterator &operator++() {
      ++pos_;
      ++iter_;
      return *this;
    }
    ClassIterator operator++(int) {
      auto copy = *this;
      ++pos_;
      ++iter_;
      return copy;
    }
    std::pair<size_t, typename Iterator::reference> operator*() {
      return std::pair<size_t, typename Iterator::reference>(pos_, *iter_);
    }

    friend bool operator==(const ClassIterator &first, const ClassIterator &second) {
      return (first.iter_ == second.iter_);
    }
    friend bool operator!=(const ClassIterator &first, const ClassIterator &second) {
      return (first.iter_ != second.iter_);
    }
  };

  using iterator = ClassIterator;

  ClassIterator begin() const {  // NOLINT
    return ClassIterator(begin_, 0);
  }
  ClassIterator end() const {  // NOLINT
    return ClassIterator(end_);
  }
};

template <class Container>
auto Enumerate(Container &container) {
  return EnumerateProxy<typename Container::iterator, Container>(container.begin(), container.end());
}

template <class Container>
auto Enumerate(const Container &container) {
  return EnumerateProxy<typename Container::const_iterator, Container>(container.cbegin(), container.cend());
}

template <class Container>
auto Enumerate(Container &&container) {
  return EnumerateProxy<typename Container::iterator, Container>(container);
}

