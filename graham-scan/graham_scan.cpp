#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>

namespace geometry {

class Vector {
 public:
  Vector();
  Vector(int64_t x, int64_t y);
  Vector(const Vector &) = default;

  Vector &operator=(const Vector &) = default;

  friend Vector operator+(const Vector &);
  friend Vector operator-(const Vector &);

  int64_t GetX() const;
  int64_t GetY() const;

  friend Vector operator+(const Vector &, const Vector &);
  friend Vector operator-(const Vector &, const Vector &);

  friend Vector operator*(const Vector &vec, const int64_t &val);
  friend Vector operator/(const Vector &vec, const int64_t &val);

  friend Vector &operator+=(Vector &, const Vector &);
  friend Vector &operator-=(Vector &, const Vector &);

  friend Vector &operator*=(Vector &, const int64_t &);
  friend Vector &operator/=(Vector &, const int64_t &);

  friend bool operator==(const Vector &, const Vector &);

  friend int64_t CrossProduct(const Vector &, const Vector &);

 private:
  int64_t x_;
  int64_t y_;
};

Vector::Vector() : x_(0), y_(0) {
}
Vector::Vector(int64_t x, int64_t y) : x_(x), y_(y) {
}

Vector operator+(const Vector &vec) {
  return {vec.x_, vec.y_};
}
Vector operator-(const Vector &vec) {
  return {-vec.x_, -vec.y_};
}

Vector operator+(const Vector &vec1, const Vector &vec2) {
  return {vec1.x_ + vec2.x_, vec1.y_ + vec2.y_};
}
Vector operator-(const Vector &vec1, const Vector &vec2) {
  return {vec1.x_ - vec2.x_, vec1.y_ - vec2.y_};
}

Vector operator*(const Vector &vec, const int64_t &val) {
  return {vec.x_ * val, vec.y_ * val};
}
Vector operator/(const Vector &vec, const int64_t &val) {
  return {vec.x_ / val, vec.y_ / val};  // возможно деление на 0
}

Vector &operator+=(Vector &vec1, const Vector &vec2) {
  vec1.x_ += vec2.x_;
  vec1.y_ += vec2.y_;
  return vec1;
}
Vector &operator-=(Vector &vec1, const Vector &vec2) {
  vec1.x_ -= vec2.x_;
  vec1.y_ -= vec2.y_;
  return vec1;
}

Vector &operator*=(Vector &vec, const int64_t &val) {
  vec.x_ *= val;
  vec.y_ *= val;
  return vec;
}
Vector &operator/=(Vector &vec, const int64_t &val) {  // возможно деление на 0
  vec.x_ /= val;
  vec.y_ /= val;
  return vec;
}

bool operator==(const Vector &vec1, const Vector &vec2) {
  return (vec1.x_ == vec2.x_ && vec1.y_ == vec2.y_);
}

int64_t Vector::GetX() const {
  return x_;
}
int64_t Vector::GetY() const {
  return y_;
}

int64_t CrossProduct(const Vector &lhs, const Vector &rhs) {
  return lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_;
}

class Point {

 public:
  Point();
  Point(const int64_t &, const int64_t &);
  Point(const Point &other) = default;

  Point &Move(const Vector &);

  int64_t GetX() const;
  int64_t GetY() const;

  friend Vector operator-(const Point &, const Point &);
  friend Point &operator+=(Point &, const Point &);
  friend Point &operator+=(Point &, const Vector &);
  friend Point operator+(const Point &, const Vector &);

  Point &operator=(const Point &) = default;

  friend bool operator==(const Point &, const Point &);

 private:
  int64_t x_;
  int64_t y_;
};

Point::Point() : x_(0), y_(0) {
}

Point::Point(const int64_t &x, const int64_t &y) : x_(x), y_(y) {
}

Point &Point::Move(const Vector &shift) {
  x_ += shift.GetX();
  y_ += shift.GetY();
  return *this;
}

Vector operator-(const Point &lhs, const Point &rhs) {
  return {lhs.x_ - rhs.x_, lhs.y_ - rhs.y_};
}

Point &operator+=(Point &lhs, const Point &rhs) {
  lhs.x_ += rhs.x_;
  lhs.y_ += rhs.y_;
  return lhs;
}
Point &operator+=(Point &lhs, const Vector &rhs) {
  lhs.x_ += rhs.GetX();
  lhs.y_ += rhs.GetY();
  return lhs;
}

int64_t Point::GetX() const {
  return x_;
}
int64_t Point::GetY() const {
  return y_;
}

Point operator+(const Point &point, const Vector &vec) {
  Point res(point);
  return res += vec;
}

bool operator==(const Point &lhs, const Point &rhs) {
  return (lhs.GetX() == rhs.GetX() && lhs.GetY() == rhs.GetY());
}

}  // namespace geometry

inline int64_t SearchDistance(const geometry::Point &other) {
  return other.GetX() * other.GetX() + other.GetY() * other.GetY();
}

inline bool Cmp(const geometry::Point &lhs, const geometry::Point &rhs) {
  geometry::Vector first_vec = lhs - geometry::Point(0, 0);
  geometry::Vector second_vec = rhs - geometry::Point(0, 0);
  if (geometry::CrossProduct(first_vec, second_vec) == 0) {
    int64_t dis1 = SearchDistance(lhs);
    int64_t dis2 = SearchDistance(rhs);
    return (dis1 >= dis2);
  }
  bool a = geometry::CrossProduct(first_vec, second_vec) <= 0;
  return a;
}

inline size_t LeftMost(std::vector<geometry::Point> &peaks) {
  size_t res_idx = 0;
  size_t len = peaks.size();
  for (size_t i = 1; i < len; ++i) {
    if (peaks[i].GetX() <= peaks[res_idx].GetX()) {
      if (peaks[i].GetX() == peaks[res_idx].GetX()) {
        if (peaks[i].GetY() <= peaks[res_idx].GetY()) {
          res_idx = i;
        }
      } else {
        res_idx = i;
      }
    }
  }
  return res_idx;
}

inline void Print(std::vector<geometry::Point> &hull, const size_t &len, const int64_t &area) {
  std::cout << len << '\n';

  for (auto &now : hull) {
    std::cout << now.GetX() << ' ' << now.GetY() << '\n';
  }
  if (area % 2 == 0) {
    area > 0 ? std::cout << std::fixed << std::setprecision(1) << area / 2 << ".0"
             : std::cout << std::fixed << std::setprecision(1) << -area / 2 << ".0";
  } else {
    area > 0 ? std::cout << std::fixed << std::setprecision(1) << area / 2 << ".5"
             : std::cout << std::fixed << std::setprecision(1) << -area / 2 << ".5";
  }
}

inline int64_t Area(std::vector<geometry::Point> &hull, const size_t &len) {
  int64_t area = 0;
  for (size_t i = 0; i < len; ++i) {
    geometry::Vector vec1(hull[i].GetX(), hull[i].GetY());
    geometry::Vector vec2(hull[(i + 1) % len].GetX(), hull[(i + 1) % len].GetY());
    area += geometry::CrossProduct(vec1, vec2);
  }
  return area;
}

inline std::vector<geometry::Point> NewHull(std::vector<geometry::Point> &old, const size_t len) {
  std::vector<geometry::Point> new_hull;
  new_hull.emplace_back(old[0]);

  for (size_t i = 2; i <= len; ++i) {
    geometry::Vector vec1 = old[i - 2] - old[i - 1];
    geometry::Vector vec2 = old[i - 1] - old[i % len];
    if (geometry::CrossProduct(vec1, vec2) != 0) {
      new_hull.emplace_back(old[i - 1]);
    }
  }
  return new_hull;
}

inline std::vector<geometry::Point> GrahamScan(std::vector<geometry::Point> &peaks) {
  size_t left = LeftMost(peaks);

  geometry::Vector shift_vec = geometry::Point(0, 0) - peaks[left];

  for (auto &now : peaks) {
    now.Move(shift_vec);
  }

  std::swap(peaks[0], peaks[left]);

  sort(peaks.begin() + 1, peaks.end(), Cmp);

  for (auto &now : peaks) {
    now.Move(geometry::Vector(-shift_vec.GetX(), -shift_vec.GetY()));
  }

  std::stack<geometry::Point> convex_hull;
  convex_hull.push(peaks[0]);
  convex_hull.push(peaks[1]);

  for (size_t i = 2; i < peaks.size(); ++i) {

    if (peaks[i] == convex_hull.top()) {
      continue;
    }

    while (!convex_hull.empty()) {
      geometry::Point temp = convex_hull.top();
      geometry::Vector temp1 = peaks[i] - convex_hull.top();
      convex_hull.pop();
      geometry::Vector temp2 = temp - convex_hull.top();
      if (geometry::CrossProduct(temp1, temp2) >= 0) {
        convex_hull.push(temp);
        break;
      }
    }
    convex_hull.push(peaks[i]);
  }

  std::vector<geometry::Point> c_h;
  while (!convex_hull.empty()) {
    c_h.emplace_back(convex_hull.top());
    convex_hull.pop();
  }
  for (size_t i = 0; i < c_h.size() / 2; ++i) {
    std::swap(c_h[i], c_h[c_h.size() - i - 1]);
  }

  return c_h;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int64_t size;
  std::cin >> size;

  std::vector<geometry::Point> peaks;

  for (int64_t i = 0; i < size; ++i) {
    int64_t x, y;
    std::cin >> x >> y;
    peaks.emplace_back(geometry::Point(x, y));
  }

  std::vector<geometry::Point> convex_hull = GrahamScan(peaks);
  size_t len = convex_hull.size();

  std::vector<geometry::Point> new_hull = NewHull(convex_hull, len);

  size_t new_len = new_hull.size();

  int64_t area = Area(new_hull, new_len);

  Print(new_hull, new_len, area);

  return 0;
}
