#pragma once
#include <string>
#include "shape.h"

namespace geometry {

class Line : public IShape {
 public:
  Line(const Point &, const Point &);
  Line(const int64_t &, const int64_t &, const int64_t &);

  Line &Move(const Vector &) override;
  Line *Clone() const override;

  std::string ToString() const override;

  bool ContainsPoint(const Point &) const override;
  bool CrossesSegment(const Segment &) const override;

  int64_t PointValue(const Point &) const;

  int64_t GetA() const;
  int64_t GetB() const;

  Vector GetNormalVector() const;

 private:
  int64_t a_;
  int64_t b_;
  int64_t c_;
};

}  // namespace geometry