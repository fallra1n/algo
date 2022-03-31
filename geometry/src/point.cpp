#include "../point.h"
#include "../segment.h"
#include "../vector.h"
#include <string>

namespace geometry {

Point::Point() : x_(0), y_(0) {
}

Point::Point(const int64_t &x, const int64_t &y) : x_(x), y_(y) {
}
Point::Point(const Point &other) : x_(other.x_), y_(other.y_) {
}

Point &Point::Move(const Vector &shift) {
  x_ += shift.GetX();
  y_ += shift.GetY();
  return *this;
}
Point *Point::Clone() const {
  return new Point(x_, y_);
}

bool Point::ContainsPoint(const Point &other) const {
  return (x_ == other.x_ && y_ == other.y_);
}
bool Point::CrossesSegment(const Segment &other) const {
  Vector temp_vec1 = {other.GetEnd() - *this};
  Vector temp_vec2 = {*this - other.GetBegin()};
  return !(CrossProduct(temp_vec1, temp_vec2)) && DotProduct(temp_vec1, temp_vec2) >= 0;
}

std::string Point::ToString() const {
  std::string res = "Point(";
  res += std::to_string(x_);
  res += ", ";
  res += std::to_string(y_);
  res += ")";
  return res;
}

Point &Point::operator=(const Point &other) {
  x_ = other.x_;
  y_ = other.y_;
  return *this;
}

Vector operator-(const Point &lhs, const Point &rhs) {
  return {lhs.x_ - rhs.x_, lhs.y_ - rhs.y_};
}

Point &operator+=(Point &lhs, const Point &rhs) {
  lhs.x_ += rhs.x_;
  lhs.y_ += rhs.y_;
  return lhs;
}
Point &operator+=(Point &lhs, const Vector &rhs) {
  lhs.x_ += rhs.GetX();
  lhs.y_ += rhs.GetY();
  return lhs;
}

int64_t Point::GetX() const {
  return x_;
}
int64_t Point::GetY() const {
  return y_;
}
Point operator+(const Point &point, const Vector &vec) {
  Point res(point);
  return res += vec;
}

bool operator==(const Point &lhs, const Point &rhs) {
  return (lhs.GetX() == rhs.GetX() && lhs.GetY() == rhs.GetY());
}

}  // namespace geometry
