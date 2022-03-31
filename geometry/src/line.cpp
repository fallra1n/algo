#include "../line.h"
#include "../point.h"
#include "../segment.h"
#include "../vector.h"

namespace geometry {
Line::Line(const Point &p1, const Point &p2) {
  if (!(p1.GetY() - p2.GetY())) {

    a_ = 0;
    b_ = 1;
    c_ = -p1.GetY();

  } else if (!(p1.GetX() - p2.GetX())) {  // Если считать, что точки не совпадают!

    a_ = 1;
    b_ = 0;
    c_ = -p1.GetX();

  } else {

    Vector guide_vector = p2 - p1;
    a_ = -guide_vector.GetY();
    b_ = guide_vector.GetX();
    c_ = -p1.GetX() * a_ - p1.GetY() * b_;
  }
}
Line::Line(const int64_t &a, const int64_t &b, const int64_t &c) : a_(a), b_(b), c_(c) {
}

Line &Line::Move(const Vector &shift) {
  if (a_ == 0) {

    c_ -= shift.GetY();

  } else if (b_ == 0) {  // Если считать, что точки не совпадают!

    c_ -= shift.GetX();

  } else {

    c_ = c_ - a_ * shift.GetX() - b_ * shift.GetY();
  }

  return *this;
}

bool Line::ContainsPoint(const Point &point) const {
  return a_ * point.GetX() + b_ * point.GetY() + c_ == 0;
}
bool Line::CrossesSegment(const Segment &seg) const {
  int64_t val_begin = a_ * seg.GetBegin().GetX() + b_ * seg.GetBegin().GetY() + c_;
  int64_t val_end = a_ * seg.GetEnd().GetX() + b_ * seg.GetEnd().GetY() + c_;

  return (val_begin * val_end <= 0);
}

Line *Line::Clone() const {
  return new Line(a_, b_, c_);
}

std::string Line::ToString() const {
  std::string res = "Line(";
  res += std::to_string(a_);
  res += ", ";
  res += std::to_string(b_);
  res += ", ";
  res += std::to_string(c_);
  res += ")";
  return res;
}

int64_t Line::PointValue(const Point &point) const {
  return a_ * point.GetX() + b_ * point.GetY() + c_;
}
int64_t Line::GetA() const {
  return a_;
}
int64_t Line::GetB() const {
  return b_;
}

Vector Line::GetNormalVector() const {
  return {a_, b_};
}

}  // namespace geometry