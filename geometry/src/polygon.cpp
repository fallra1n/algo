#include "../polygon.h"
#include "../point.h"
#include "../segment.h"
#include "../ray.h"

namespace geometry {

Polygon::Polygon(std::vector<Point> peaks) : peaks_(peaks) {
}

Polygon &Polygon::Move(const Vector &shift) {
  for (size_t i = 0; i < peaks_.size(); ++i) {
    peaks_[i] += shift;
  }
  return *this;
}
Polygon *Polygon::Clone() const {
  return new Polygon(peaks_);
}

bool Polygon::ContainsPoint(const Point &point) const {
  Point temp_point(10001, point.GetY() + 1);
  Ray temp_ray(point, temp_point);
  int64_t count = 0;
  for (size_t i = 0; i < peaks_.size() - 1; ++i) {
    if (Segment(peaks_[i], peaks_[i + 1]).ContainsPoint(temp_ray.GetBegin())) {
      return true;
    }
    if (temp_ray.CrossesSegment({peaks_[i], peaks_[i + 1]})) {
      ++count;
    }
  }
  if (temp_ray.CrossesSegment({peaks_[0], peaks_[peaks_.size() - 1]})) {
    if (Segment(peaks_[0], peaks_[peaks_.size() - 1]).ContainsPoint(temp_ray.GetBegin())) {
      return true;
    }
    ++count;
  }

  return count % 2;
}

bool Polygon::CrossesSegment(const Segment &seg) const {
  for (size_t i = 0; i < peaks_.size() - 1; ++i) {
    if (seg.CrossesSegment({peaks_[i], peaks_[i + 1]})) {
      return true;
    }
    if (seg.CrossesSegment({peaks_[peaks_.size() - 1], peaks_[0]})) {
      return true;
    }
  }
  return false;
}
std::string Polygon::ToString() const {
  std::string res = "Polygon(";
  for (size_t i = 0; i < peaks_.size(); ++i) {
    res += peaks_[i].ToString();
    if (i != peaks_.size() - 1) {
      res += ", ";
    }
  }
  res += ")";
  return res;
}

}  // namespace geometry