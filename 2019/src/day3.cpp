#include <bits/stdc++.h>

#include "util.h"

namespace {
class Point {
 public:
  int x;
  int y;

  int distance() const {
    return abs(x) + abs(y);    
  }

  bool operator==(const Point &other) const {
    return (x == other.x) && (y == other.y);
  }
};
}  // namespace

namespace std {

template <>
struct hash<Point> {
  std::size_t operator()(const Point &p) const {
    using std::hash;

    return (hash<int>()(p.x) ^ (hash<int>()(p.y) << 1));
  }
};

}  // namespace std

void makePointVect(const std::string &steps, std::vector<Point> &outVect) {
  std::stringstream ss{steps};
  std::string token;

  Point pos{0, 0};
  outVect.clear();

  while (getline(ss, token, ',')) {
    int len = std::stoi(token.substr(1));
    int dx{0}, dy{0};

    switch (token[0]) {
      case 'D':
        dy = -1;
        break;
      case 'U':
        dy = 1;
        break;
      case 'L':
        dx = -1;
        break;
      case 'R':
        dx = 1;
        break;
    }

    for (int i{0}; i < len; ++i) {
      pos.x += dx;
      pos.y += dy;
      outVect.push_back(pos);
    }
  }
}

void makePointSet(const std::string &steps, std::unordered_set<Point> &outSet) {
  std::vector<Point> vect;
  makePointVect(steps, vect);

  outSet.clear();
  outSet.insert(vect.begin(), vect.end());
}

int day3_1(const Stringlist &input) {
  std::vector<Point> wire1;
  std::unordered_set<Point> wire2;
  int min_dist{INT_MAX};

  makePointVect(input.get(0), wire1);
  makePointSet(input.get(1), wire2);

  for(auto &p: wire1){
    if(wire2.count(p) > 0){
      min_dist = std::min(min_dist, p.distance());
    }
  }
  return min_dist;
}

int day3_2(const Stringlist &input) {
  std::vector<Point> wire1;
  std::vector<Point> wire2;
  std::unordered_map<Point, size_t> mapWire2;


  int min_step{INT_MAX};

  makePointVect(input.get(0), wire1);
  makePointVect(input.get(1), wire2);

  for(size_t i{0}; i < wire2.size(); ++i){
    mapWire2[wire2[i]] = i;
  }

  for(size_t i{0}; i < wire1.size(); ++i){
    auto &p {wire1[i]};
    if(mapWire2.count(p) > 0){
      min_step = std::min(min_step, static_cast<int>(i + mapWire2[p] + 2));
    }
  }
  return min_step;
}

void day3(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      runTest(day3_1(slist.fromString("R8,U5,L5,D3\nU7,R6,D4,L4")), 6);
      runTest(day3_1(slist.fromString("R75,D30,R83,U83,L12,D49,R71,U7,L72\nU62,R66,U55,R34,D71,R55,D58,R83")), 159);
      runTest(day3_1(slist.fromString("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\nU98,R91,D20,R16,D67,R40,U7,R15,U6,R7")), 135);
    } else {
      runTest(day3_2(slist.fromString("R8,U5,L5,D3\nU7,R6,D4,L4")), 30);
      runTest(day3_2(slist.fromString("R75,D30,R83,U83,L12,D49,R71,U7,L72\nU62,R66,U55,R34,D71,R55,D58,R83")), 610);
      runTest(day3_2(slist.fromString("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\nU98,R91,D20,R16,D67,R40,U7,R15,U6,R7")), 410);
    }
  } else {
    if (slist.fromFile(filename)) {
      int result;

      result = (part == 1) ? day3_1(slist) : day3_2(slist);
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}