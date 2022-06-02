#pragma once
#include <stdexcept>
#include <memory>

class BadAnyCast : public std::bad_cast {
 public:
  [[nodiscard]] const char *what() const noexcept override {
    return "BadAnyCast error";
  }
};

class IHolder {
 public:
  virtual IHolder *Clone() const = 0;
  virtual ~IHolder() = default;
};

template <typename T>
class AnyHolder final : public IHolder {
 public:
  AnyHolder(const T &value) : value_(value) {  // NOLINT
  }

  AnyHolder(T &&value) noexcept : value_(value) {  // NOLINT
  }

  AnyHolder *Clone() const final {
    auto *copy = new AnyHolder<T>(value_);
    return copy;
  }

  ~AnyHolder() final = default;

  T value_;
};

class Any {
 public:
  Any() = default;

  Any(const Any &other) : storage_(other.storage_->Clone()) {
  }

  Any(Any &&other) noexcept : storage_(other.storage_->Clone()) {
  }

  template <typename T>
  Any(T &&other) noexcept {  // NOLINT
    storage_ = static_cast<std::unique_ptr<IHolder>>((new AnyHolder<T>(std::forward<T>(other))));
  }

  template <typename T>
  Any &operator=(T &&other) {
    storage_ = static_cast<std::unique_ptr<IHolder>>((new AnyHolder<T>(std::forward<T>(other))));
    return *this;
  }

  Any &operator=(Any &other) {
    if (this != &other) {
      storage_ = static_cast<std::unique_ptr<IHolder>>(other.storage_->Clone());
    }
    return *this;
  }

  Any &operator=(Any &&other) {
    storage_ = static_cast<std::unique_ptr<IHolder>>(other.storage_->Clone());
    other.storage_ = nullptr;
    return *this;
  }

  void Reset() {
    storage_ = nullptr;
  }

  void Swap(Any &other) {
    std::swap(storage_, other.storage_);
  }

  bool HasValue() const {
    return storage_.operator bool();
  }

  auto GetStorage() const {
    return storage_.get();
  }

 private:
  std::unique_ptr<IHolder> storage_ = nullptr;
};

template <typename T>
T AnyCast(const Any &value) {

  if (dynamic_cast<AnyHolder<T> *>(value.GetStorage()) == nullptr) {
    throw BadAnyCast();
  }

  T copy = dynamic_cast<AnyHolder<T> *>((value.GetStorage()))->value_;

  return copy;
}
