#include <iostream>
#include <vector>
#include <limits>

const int64_t kNegInf = std::numeric_limits<int>::min();

int Left(int i) {
  return 2 * i;
}

int Right(int i) {
  return 2 * i + 1;
}

int Check(int l, int r, int a, int b) {
  if (a >= l && b <= r) {
    return 0;
  }
  if (((a > r) && (b > r))  ((b < l) && (a < l))) {
    return 1;
  }
  return 2;
}

class SegmentTree {
 public:
  explicit SegmentTree(const std::vector<int> &, int);

  void Update(int, int, int, int, int, int, int);

  int GetMaxOnSegment(int, int, int, int, int, int);

 private:
  struct Node {
    int max = kNegInf;
    int promise = kNegInf;
  };

  std::vector<Node> data_;

  void BuildTree(const std::vector<int> &);

  void Push(int);
};

SegmentTree::SegmentTree(const std::vector<int> &values, int len_extended) {
  data_.resize(2 * len_extended, {kNegInf, kNegInf});
  BuildTree(values);
}

void SegmentTree::BuildTree(const std::vector<int> &values) {
  int mid = static_cast<int>(data_.size()) / 2;
  for (int i = 0; i < static_cast<int>(values.size()); ++i) {
    data_[i + mid] = {values[i], kNegInf};
  }
  for (int i = mid - 1; i >= 1; --i) {
    data_[i] = {std::max(data_[Left(i)].max, data_[Right(i)].max), kNegInf};
  }
}

void SegmentTree::Push(int v) {
  if (data_[v].promise != kNegInf) {
    data_[v].max += data_[v].promise;
    data_[Left(v)].promise == kNegInf ? data_[Left(v)].promise = data_[v].promise
                                      : data_[Left(v)].promise += data_[v].promise;
    data_[Right(v)].promise == kNegInf ? data_[Right(v)].promise = data_[v].promise
                                       : data_[Right(v)].promise += data_[v].promise;
    data_[v].promise = kNegInf;
  }
}

void SegmentTree::Update(int l, int r, int v, int a, int b, int value, int len) {
  if (Check(l, r, a, b) == 1  a > b || v >= len) {
    return;
  }
  if (Check(l, r, a, b) == 0) {
    if (data_[v].promise != kNegInf) {
      data_[v].promise += value;
    } else {
      data_[v].promise = value;
    }
    return;
  }

  if (Left(v) >= len) {
    return;
  }
  Push(v);

  int mid = (a + b) / 2;
  Update(l, r, Left(v), a, mid, value, len);
  Update(l, r, Right(v), mid + 1, b, value, len);
  int new_left, new_right;

  if (data_[Left(v)].promise != kNegInf) {
    new_left = data_[Left(v)].max + data_[Left(v)].promise;
  } else {
    new_left = data_[Left(v)].max;
  }

  if (data_[Right(v)].promise != kNegInf) {
    new_right = data_[Right(v)].max + data_[Right(v)].promise;
  } else {
    new_right = data_[Right(v)].max;
  }

  data_[v].max = std::max(new_left, new_right);
}

int SegmentTree::GetMaxOnSegment(int l, int r, int v, int a, int b, int len) {
  if (Check(l, r, a, b) == 0) {
    if (data_[v].promise != kNegInf) {
      return data_[v].max + data_[v].promise;
    }
    return data_[v].max;
  }

  if (Check(l, r, a, b) == 1) {
    return kNegInf;
  }

  Push(v);
  int mid = (a + b) / 2;
  int left_res = GetMaxOnSegment(l, r, Left(v), a, mid, len);
  int right_res = GetMaxOnSegment(l, r, Right(v), mid + 1, b, len);
  return std::max(left_res, right_res);
}

std::istream &operator>>(std::istream &in, std::vector<int> &data) {
  for (int &i : data) {
    in >> i;
  }
  return in;
}

int main() {
  int n;
  std::cin >> n;

  std::vector<int> values(n);
  std::cin >> values;

  int len_extended = 1;
  while (len_extended < static_cast<int>(values.size())) {
    len_extended *= 2;
  }

  SegmentTree tree(values, len_extended);

  int size;
  std::cin >> size;

  char cmd;
  for (int i = 0; i < size; ++i) {
    std::cin >> cmd;

    if (cmd == 'm') {
      int l, r;
      std::cin >> l >> r;
      std::cout << tree.GetMaxOnSegment(l, r, 1, 1, static_cast<int>(len_extended), 2 * len_extended) << ' ';
    } else {
      int l, r, value;
      std::cin >> l >> r >> value;

      tree.Update(l, r, 1, 1, static_cast<int>(len_extended), value, 2 * len_extended);
    }
  }

  return 0;
}