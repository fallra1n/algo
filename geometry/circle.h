#pragma once
#include <string>
#include "point.h"
#include "shape.h"

namespace geometry {
class Circle : public IShape {
 public:
  Circle(const Point &, const int64_t &);
  Circle(const Circle &);

  Circle &Move(const Vector &) override;
  Circle *Clone() const override;

  bool ContainsPoint(const Point &) const override;
  bool CrossesSegment(const Segment &) const override;
  bool StrictContainsPoint(const Point &) const;

  std::string ToString() const override;

 private:
  Point center_;
  int64_t radius_;
};

}  // namespace geometry