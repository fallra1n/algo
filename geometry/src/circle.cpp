#include "../circle.h"
#include "../line.h"
#include "../segment.h"
#include "../vector.h"

namespace geometry {

Circle::Circle(const Point &other_center, const int64_t &other_rad) : center_(other_center), radius_(other_rad) {
}
Circle::Circle(const Circle &other) : center_(other.center_), radius_(other.radius_) {
}

Circle &Circle::Move(const Vector &other) {
  center_ += other;
  return *this;
}
Circle *Circle::Clone() const {
  return new Circle(center_, radius_);
}

bool Circle::ContainsPoint(const Point &other) const {

  int64_t first = (other.GetX() - center_.GetX()) * (other.GetX() - center_.GetX());
  int64_t second = (other.GetY() - center_.GetY()) * (other.GetY() - center_.GetY());
  int64_t deg_rad = radius_ * radius_;

  return (first + second <= deg_rad);
}

bool Circle::StrictContainsPoint(const Point &other) const {

  int64_t first = (other.GetX() - center_.GetX()) * (other.GetX() - center_.GetX());
  int64_t second = (other.GetY() - center_.GetY()) * (other.GetY() - center_.GetY());
  int64_t deg_rad = radius_ * radius_;

  return (first + second < deg_rad);
}

bool Circle::CrossesSegment(const Segment &seg) const {

  if (ContainsPoint(seg.GetBegin()) && !ContainsPoint(seg.GetEnd())) {
    return true;
  }

  if (!ContainsPoint(seg.GetBegin()) && ContainsPoint(seg.GetEnd())) {
    return true;
  }

  if (StrictContainsPoint(seg.GetBegin()) && StrictContainsPoint(seg.GetEnd())) {
    return false;
  }

  Line other_line(seg.GetBegin(), seg.GetEnd());

  int64_t val = other_line.PointValue(center_);

  int64_t len = val * val;

  if (len > radius_ * radius_ * (other_line.GetA() * other_line.GetA() + other_line.GetB() * other_line.GetB())) {
    return false;
  }

  Vector other_normal = other_line.GetNormalVector();

  Line normal_line(center_, center_ + other_normal);

  return normal_line.CrossesSegment(seg);
}
std::string Circle::ToString() const {
  std::string res = "Circle(";
  res += center_.ToString();
  res += ", ";
  res += std::to_string(radius_);
  res += ")";
  return res;
}

}  // namespace geometry
