#pragma once
#include <string>
#include "point.h"
#include "shape.h"

namespace geometry {

class Segment : public geometry::IShape {
 public:
  Segment(const Point &, const Point &);

  Segment &Move(const Vector &) override;
  Segment *Clone() const override;

  bool ContainsPoint(const Point &) const override;
  bool CrossesSegment(const Segment &) const override;

  std::string ToString() const override;

  Point GetBegin() const;
  Point GetEnd() const;

 private:
  Point begin_;
  Point end_;
};

}  // namespace geometry