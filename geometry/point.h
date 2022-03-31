#pragma once
#include <string>
#include "shape.h"

namespace geometry {

class Point : public geometry::IShape {

 public:
  Point();
  Point(const int64_t &, const int64_t &);
  Point(const Point &other);

  Point &Move(const Vector &) override;
  Point *Clone() const override;
  bool ContainsPoint(const Point &) const override;
  bool CrossesSegment(const Segment &) const override;
  std::string ToString() const override;

  int64_t GetX() const;
  int64_t GetY() const;

  friend Vector operator-(const Point &, const Point &);
  friend Point &operator+=(Point &, const Point &);
  friend Point &operator+=(Point &, const Vector &);
  friend Point operator+(const Point &, const Vector &);

  Point &operator=(const Point &);

  friend bool operator==(const Point &, const Point &);

 private:
  int64_t x_;
  int64_t y_;
};

}  // namespace geometry
