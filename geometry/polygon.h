#pragma once
#include <string>
#include <vector>
#include "shape.h"

namespace geometry {

class Polygon : public IShape {
 public:
  explicit Polygon(std::vector<Point>);

  Polygon &Move(const Vector &) override;
  bool ContainsPoint(const Point &) const override;
  bool CrossesSegment(const Segment &) const override;
  Polygon *Clone() const override;
  std::string ToString() const override;

 private:
  std::vector<Point> peaks_;
};

}  // namespace geometry