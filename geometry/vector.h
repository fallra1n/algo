#pragma once
#include <cstdint>
#include <string>

namespace geometry {

class Vector {
 public:
  Vector();
  Vector(int64_t x, int64_t y);
  Vector(const Vector &) = default;

  Vector &operator=(const Vector &) = default;

  std::string ToString() const;

  friend Vector operator+(const Vector &);
  friend Vector operator-(const Vector &);

  int64_t GetX() const;
  int64_t GetY() const;

  friend Vector operator+(const Vector &, const Vector &);
  friend Vector operator-(const Vector &, const Vector &);

  friend Vector operator*(const Vector &vec, const int64_t &val);
  friend Vector operator/(const Vector &vec, const int64_t &val);

  friend Vector operator+=(Vector &, const Vector &);
  friend Vector operator-=(Vector &, const Vector &);

  friend Vector operator*=(Vector &, const int64_t &);
  friend Vector operator/=(Vector &, const int64_t &);

  friend bool operator==(const Vector &, const Vector &);

  friend int64_t CrossProduct(const Vector &, const Vector &);
  friend int64_t DotProduct(const Vector &, const Vector &);

 private:
  int64_t x_;
  int64_t y_;
};
}  // namespace geometry
