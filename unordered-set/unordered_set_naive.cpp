#pragma once
#include <iostream>
#include <vector>
#include <list>

template <typename KeyT>
class UnorderedSet {
 public:
  UnorderedSet() : n_elements_(0), n_buckets_(0) {
  }

  explicit UnorderedSet(size_t count) : buffer_(count), n_elements_(0), n_buckets_(count) {
  }

  template <typename ForwardIt>
  UnorderedSet(const ForwardIt &first, const ForwardIt &last) : n_elements_(0), n_buckets_(0) {
    for (auto i = first; i != last; ++i) {
      buffer_.emplace_back(std::list<KeyT>());
      ++n_buckets_;
      ++n_elements_;
    }

    for (auto i = first; i != last; ++i) {
      auto pos = std::hash<KeyT>{}(*i) % n_buckets_;
      buffer_[pos].push_back(*i);
    }
  }

  UnorderedSet(const UnorderedSet<KeyT> &other) : buffer_(other.n_buckets_) {
    buffer_ = other.buffer_;
    n_buckets_ = other.n_buckets_;
    n_elements_ = other.n_elements_;
  }
  UnorderedSet(UnorderedSet<KeyT> &&other) {
    buffer_ = std::move(other.buffer_);
    n_buckets_ = other.n_buckets_;
    n_elements_ = other.n_elements_;
    other.n_buckets_ = 0;
    other.n_elements_ = 0;
  }

  UnorderedSet &operator=(const UnorderedSet &other) {
    if (this != &other) {
      UnorderedSet<KeyT>(other).Swap(*this);
    }
    return *this;
  }
  UnorderedSet &operator=(UnorderedSet &&other) noexcept {
    UnorderedSet<KeyT>(std::move(other)).Swap(*this);
    return *this;
  }

  void Swap(UnorderedSet<KeyT> &other) {
    std::swap(buffer_, other.buffer_);
    std::swap(n_buckets_, other.n_buckets_);
    std::swap(n_elements_, other.n_elements_);
  }

  size_t Size() {
    return n_elements_;
  }
  size_t Size() const {
    return n_elements_;
  }

  bool Empty() {
    return (n_elements_ == 0);
  }
  bool Empty() const {
    return (n_elements_ == 0);
  }

  void Clear() {
    buffer_.clear();
    n_elements_ = 0;
    n_buckets_ = 0;
  }

  double LoadFactor() {
    if (n_buckets_ == 0) {
      return n_buckets_;
    }
    return static_cast<double>(n_elements_) / static_cast<double>(n_buckets_);
  }
  double LoadFactor() const {
    if (n_buckets_ == 0) {
      return n_buckets_;
    }
    return static_cast<double>(n_elements_) / static_cast<double>(n_buckets_);
  }

  size_t BucketCount() {
    return n_buckets_;
  }
  size_t BucketCount() const {
    return n_buckets_;
  }

  size_t BucketSize(size_t id) const {
    if (id >= n_buckets_) {
      return 0;
    }
    return buffer_[id].size();
  }

  size_t Bucket(const KeyT &key) const {
    return ((std::hash<KeyT>{}(key)) % n_buckets_);
  }

  bool Find(const KeyT &key) const {
    if (n_buckets_ == 0) {
      return static_cast<bool>(n_buckets_);
    }
    size_t pos = std::hash<KeyT>{}(key) % n_buckets_;
    if (buffer_[pos].empty()) {
      return !buffer_[pos].empty();
    }
    for (auto i = buffer_[pos].begin(); i != buffer_[pos].end(); ++i) {
      if (key == *i) {
        return true;
      }
    }
    return false;
  }

  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count != n_buckets_ && new_bucket_count >= n_elements_) {
      std::vector<std::list<KeyT>> new_table(new_bucket_count);

      for (size_t i = 0; i < n_buckets_; ++i) {
        if (!buffer_[i].empty()) {
          while (true) {
            auto temp = buffer_[i].begin();
            auto idx = std::hash<KeyT>{}(*temp) % new_bucket_count;
            auto next = temp;
            ++next;
            new_table[idx].splice(new_table[idx].end(), buffer_[i], temp, next);
            if (next == buffer_[i].end()) {
              break;
            }
          }
        }
      }
      buffer_ = std::move(new_table);
      n_buckets_ = new_bucket_count;
    }
  }

  void Reserve(size_t new_bucket_count) {
    if (new_bucket_count > n_buckets_) {
      std::vector<std::list<KeyT>> new_table(new_bucket_count);

      for (size_t i = 0; i < n_buckets_; ++i) {
        if (!buffer_[i].empty()) {
          while (true) {
            auto temp = buffer_[i].begin();
            auto idx = std::hash<KeyT>{}(*temp) % new_bucket_count;
            auto next = temp;
            ++next;
            new_table[idx].splice(new_table[idx].end(), buffer_[i], temp, next);
            if (next == buffer_[i].end()) {
              break;
            }
          }
        }
      }
      buffer_ = std::move(new_table);
      n_buckets_ = new_bucket_count;
    }
  }

  template <typename T>
  void Insert(T &&key) {
    if (n_elements_ + 1 > n_buckets_) {
      if (n_buckets_ == 0) {
        Rehash(1);
      } else {
        Rehash(n_buckets_ * 2);
      }
    }
    ++n_elements_;
    size_t pos = std::hash<KeyT>{}(key) % n_buckets_;
    buffer_[pos].push_back(std::forward<T>(key));
  }

  void Erase(const KeyT &key) {
    size_t pos = std::hash<KeyT>{}(key) % n_buckets_;
    for (auto i = buffer_[pos].begin(); i != buffer_[pos].end(); ++i) {
      if (*i == key) {
        buffer_[pos].erase(i);
        break;
      }
    }
    --n_elements_;
  }

 private:
  std::vector<std::list<KeyT>> buffer_;
  size_t n_elements_;
  size_t n_buckets_;
};