#include "../segment.h"
#include "../line.h"
#include "../vector.h"
#include <string>

namespace geometry {

Segment::Segment(const Point &begin, const Point &end) : begin_(begin), end_(end) {
}

Point Segment::GetBegin() const {
  return begin_;
}
Point Segment::GetEnd() const {
  return end_;
}

Segment &Segment::Move(const Vector &shift) {
  Point temp_p = {shift.GetX(), shift.GetY()};
  begin_ += temp_p;
  end_ += temp_p;
  return *this;
}
Segment *Segment::Clone() const {
  return new Segment(begin_, end_);
}

bool Segment::ContainsPoint(const Point &other) const {
  return other.CrossesSegment(*this);
}
bool Segment::CrossesSegment(const Segment &other) const {
  Line our_line(begin_, end_);
  Line other_line(other.begin_, other.end_);

  if (our_line.CrossesSegment(other) && other_line.CrossesSegment(*this)) {
    bool flag1 = std::max(begin_.GetX(), end_.GetX()) >= std::min(other.begin_.GetX(), other.end_.GetX());
    bool flag2 = std::max(begin_.GetY(), end_.GetY()) >= std::min(other.begin_.GetY(), other.end_.GetY());
    return flag1 && flag2;
  }

  return false;
}

std::string Segment::ToString() const {
  std::string res = "Segment(";
  res += begin_.ToString();
  res += ", ";
  res += end_.ToString();
  res += ")";
  return res;
}

}  // namespace geometry
