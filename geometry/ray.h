#pragma once
#include <string>
#include "vector.h"
#include "shape.h"
#include "point.h"

namespace geometry {

class Ray : public IShape {
 public:
  Ray();
  Ray(const Point &, const Point &);
  Ray(const Point &, const Vector &);
  Ray(const Ray &);

  Ray &operator=(const Ray &);

  Ray &Move(const Vector &) override;
  Ray *Clone() const override;

  bool ContainsPoint(const Point &) const override;
  bool CrossesSegment(const Segment &) const override;

  std::string ToString() const override;

  Point GetBegin() const;
  Vector GetDirection() const;

 private:
  Point begin_;
  Vector direction_;
};

}  // namespace geometry