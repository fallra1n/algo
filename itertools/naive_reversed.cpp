#pragma once
#define REVERSE_REVERSED_IMPLEMENTED

template <class Iterator, class ReverseIterator>
class Rev {
  Iterator begin_;
  Iterator end_;
  ReverseIterator rbegin_;
  ReverseIterator rend_;

 public:
  explicit Rev(Iterator begin, Iterator end, ReverseIterator rbegin, ReverseIterator rend)
      : begin_(begin), end_(end), rbegin_(rbegin), rend_(rend) {
  }

  struct iterator {  // NOLINT
   private:
    Iterator iter_;

   public:
    explicit iterator(Iterator iter) : iter_(iter) {
    }

    typename Iterator::reference operator*() {
      return *iter_;
    }

    iterator &operator++() {
      ++iter_;
      return *this;
    }

    iterator operator++(int) {
      iterator temp = *this;
      ++*this;
      return temp;
    }

    friend bool operator==(const iterator &first, const iterator &second) {
      return first.iter_ == second.iter_;
    }

    friend bool operator!=(const iterator &first, const iterator &second) {
      return first.iter_ != second.iter_;
    }
  };

  struct reverse_iterator {  // NOLINT
   private:
    ReverseIterator iter_;

   public:
    explicit reverse_iterator(ReverseIterator iter) : iter_(iter) {
    }

    typename Iterator::reference operator*() {
      return *iter_;
    }

    reverse_iterator &operator++() {
      ++iter_;
      return *this;
    }

    reverse_iterator operator++(int) {
      reverse_iterator temp = *this;
      ++*this;
      return temp;
    }

    friend bool operator==(const reverse_iterator &first, const reverse_iterator &second) {
      return first.iter_ == second.iter_;
    }

    friend bool operator!=(const reverse_iterator &first, const reverse_iterator &second) {
      return first.iter_ != second.iter_;
    }
  };

  iterator begin() const {  // NOLINT
    return iterator(begin_);
  }

  iterator end() const {  // NOLINT
    return iterator(end_);
  }

  reverse_iterator rbegin() const {  // NOLINT
    return reverse_iterator(rbegin_);
  }

  reverse_iterator rend() const {  // NOLINT
    return reverse_iterator(rend_);
  }
};

template <class Container>
auto Reversed(Container &container) {
  return Rev<typename Container::reverse_iterator, typename Container::iterator>(container.rbegin(), container.rend(),
                                                                                 container.begin(), container.end());
}

template <class Container>
auto Reversed(const Container &container) {
  return Rev<typename Container::const_reverse_iterator, typename Container::const_iterator>(
      container.crbegin(), container.crend(), container.cbegin(), container.cend());
}
