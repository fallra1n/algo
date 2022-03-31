#include "../vector.h"

namespace geometry {

Vector::Vector() : x_(0), y_(0) {
}
Vector::Vector(int64_t x, int64_t y) : x_(x), y_(y) {
}

std::string Vector::ToString() const {
  std::string res = "Vector(";
  res += std::to_string(x_);
  res += ", ";
  res += std::to_string(y_);
  res += ")";
  return res;
}

Vector operator+(const Vector &vec) {
  return {vec.x_, vec.y_};
}
Vector operator-(const Vector &vec) {
  return {-vec.x_, -vec.y_};
}

Vector operator+(const Vector &vec1, const Vector &vec2) {
  return {vec1.x_ + vec2.x_, vec1.y_ + vec2.y_};
}
Vector operator-(const Vector &vec1, const Vector &vec2) {
  return {vec1.x_ - vec2.x_, vec1.y_ - vec2.y_};
}

Vector operator*(const Vector &vec, const int64_t &val) {
  return {vec.x_ * val, vec.y_ * val};
}
Vector operator/(const Vector &vec, const int64_t &val) {
  return {vec.x_ / val, vec.y_ / val};  // возможно деление на 0
}

Vector operator+=(Vector &vec1, const Vector &vec2) {
  vec1.x_ += vec2.x_;
  vec1.y_ += vec2.y_;
  return vec1;
}
Vector operator-=(Vector &vec1, const Vector &vec2) {
  vec1.x_ -= vec2.x_;
  vec1.y_ -= vec2.y_;
  return vec1;
}

Vector operator*=(Vector &vec, const int64_t &val) {
  vec.x_ *= val;
  vec.y_ *= val;
  return vec;
}
Vector operator/=(Vector &vec, const int64_t &val) {  // возможно деление на 0
  vec.x_ /= val;
  vec.y_ /= val;
  return vec;
}

bool operator==(const Vector &vec1, const Vector &vec2) {
  return (vec1.x_ == vec2.x_ && vec1.y_ == vec2.y_);
}

int64_t Vector::GetX() const {
  return x_;
}
int64_t Vector::GetY() const {
  return y_;
}

int64_t CrossProduct(const Vector &lhs, const Vector &rhs) {
  return lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_;
}

int64_t DotProduct(const Vector &lhs, const Vector &rhs) {
  return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_;
}

}  // namespace geometry