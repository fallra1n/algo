#include "../ray.h"
#include "../line.h"
#include "../segment.h"

namespace geometry {

Ray::Ray() : begin_({0, 0}), direction_({0, 0}) {
}
Ray::Ray(const Point &begin, const Point &end) : begin_(begin), direction_(end - begin) {
}
Ray::Ray(const Point &other_beg, const Vector &other_dir) : begin_(other_beg), direction_(other_dir) {
}
Ray::Ray(const Ray &other) : begin_(other.begin_), direction_(other.direction_) {
}

Ray &Ray::Move(const Vector &shift) {
  begin_ += shift;
  return *this;
}
Ray *Ray::Clone() const {
  return new Ray(begin_, direction_);
}

bool Ray::ContainsPoint(const Point &point) const {
  Vector temp = point - begin_;
  return (!CrossProduct(temp, direction_) && DotProduct(temp, direction_) >= 0);
}
bool Ray::CrossesSegment(const Segment &seg) const {
  if (seg.ContainsPoint(begin_)) {
    return true;
  }

  Line line_vec(begin_, begin_ + direction_);

  if (line_vec.CrossesSegment(seg)) {

    Vector vec_seg = seg.GetEnd() - seg.GetBegin();
    Vector temp_vec = begin_ - seg.GetBegin();

    int64_t cross_product1 = CrossProduct(vec_seg, temp_vec);
    int64_t cross_product2 = CrossProduct(vec_seg, direction_);

    return cross_product1 * cross_product2 <= 0;
  }

  return false;
}

std::string Ray::ToString() const {
  std::string res = "Ray(";
  res += begin_.ToString();
  res += ", ";
  res += direction_.ToString();
  res += ")";
  return res;
}

Point Ray::GetBegin() const {
  return begin_;
}
Vector Ray::GetDirection() const {
  return direction_;
}

Ray &Ray::operator=(const Ray &other) {
  begin_ = other.GetBegin();
  direction_ = other.GetDirection();
  return *this;
}

}  // namespace geometry