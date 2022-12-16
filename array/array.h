#include <stdexcept>
#define ARRAY_TRAITS_IMPLEMENTED
#pragma once

class ArrayOutOfRange : public std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
  }
};

template <class T, std::size_t N>
class Array {
 public:
  T data[N];

  T& Back() {
    return data[N - 1];
  };

  const T& Back() const {
    return data[N - 1];
  };

  T& Front() {
    return data[0];
  }

  const T& Front() const {
    return data[0];
  }

  const T* Data() const {
    return data;
  }

  int Size() const {
    return N;
  }

  bool Empty() const {
    return (N == 0);
  }

  T& operator[](int pos) {
    return data[pos];
  }

  const T& operator[](int pos) const {
    return data[pos];
  }

  T& At(size_t id_x) {
    if (id_x > N - 1) {
      throw ArrayOutOfRange{};
    }
    return data[id_x];
  }

  const T& At(size_t id_x) const {
    if (id_x > N - 1) {
      throw ArrayOutOfRange{};
    }
    return data[id_x];
  }

  void Fill(const T& value) {
    for (size_t i = 0; i < N; ++i) {
      data[i] = value;
    }
  }

  void Swap(Array<T, N>& other) {
    T temp_arr[N];
    for (size_t i = 0; i < N; ++i) {
      temp_arr[i] = data[i];
    }
    for (size_t i = 0; i < N; ++i) {
      data[i] = other[i];
    }
    for (size_t i = 0; i < N; ++i) {
      other[i] = temp_arr[i];
    }
  }
};

template <class T>
size_t GetSize(const T) {
  return 0;
}

template <class T, std::size_t N>
size_t GetSize(const T (&)[N]) {
  return N;
}

template <class T>
size_t GetRank(const T) {
  return 0;
}

template <class T, std::size_t N>
size_t GetRank(const T (&arr)[N]) {
  return 1 + GetRank(arr[0]);
}

template <class T>
size_t GetNumElements(const T) {
  return 1;
}

template <class T, std::size_t N>
size_t GetNumElements(const T (&arr)[N]) {
  return N * GetNumElements(arr[0]);
}
