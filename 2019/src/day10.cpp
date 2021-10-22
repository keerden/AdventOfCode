#include <bits/stdc++.h>

#include "util.h"

namespace {

class Vect2D {
 public:
  int x, y;

  Vect2D(int x, int y) : x{x}, y{y} {}
  Vect2D(void) : x{0}, y{0} {}
  Vect2D(const Vect2D &other) : x{other.x}, y{other.y} {}

  Vect2D operator-(const Vect2D &rhs) const {
    Vect2D result(x - rhs.x, y - rhs.y);
    return result;
  }

  bool operator==(const Vect2D &rhs) const {
    return (x == rhs.x) && (y == rhs.y);
  }

  Vect2D &operator=(const Vect2D &other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &output, const Vect2D &p) {
    output << "(" << p.x << ", " << p.y << ")";
    return output;
  }

  Vect2D calcSlope(const Vect2D &other) const {
    Vect2D slope(0, 0);
    if (*this == other)
      return slope;

    slope = other - *this;

    if (slope.x == 0) {
      slope.y = (slope.y > 0) ? 1 : -1;
    } else if (slope.y == 0) {
      slope.x = (slope.x > 0) ? 1 : -1;
    } else {
      //reduce fraction by finding greatest common divider
      int divider = std::min(abs(slope.x), abs(slope.y));
      while (divider > 1) {
        if ((slope.x % divider == 0) && (slope.y % divider == 0)) {
          slope.x /= divider;
          slope.y /= divider;
          break;
        }
        --divider;
      }
    }
    return slope;
  }

  bool lessAngle(const Vect2D &other) const {
    int q = getQuadrant();
    int qo = other.getQuadrant();
    bool result{false};

    if (q != qo)
      return q < qo;

    if (q == 0 || q == 2) {
      result = ((static_cast<double>(-x) / y) < (static_cast<double>(-other.x) / other.y));
    } else {
      result = ((static_cast<double>(y) / x) < (static_cast<double>(other.y) / other.x));
    }

    return result;
  }

  bool lessMag(const Vect2D &other) const {
    int mag = (x * x) + (y * y);
    int magOther = (other.x * other.x) + (other.y * other.y);

    return mag < magOther;
  }

  int getQuadrant() const {
    int result;
    if (x > 0) {
      result = (y < 0) ? 0 : 1;
    } else if (x == 0) {
      result = (y < 0) ? 0 : 2;
    } else {
      result = (y > 0) ? 2 : 3;
    }
    return result;
  }
};

class Vect2D_compAngle {
 public:
  bool operator()(const Vect2D &lhs, const Vect2D &rhs) const {
    return lhs.lessAngle(rhs);
  }
};
}  // namespace

namespace std {

template <>
struct hash<Vect2D> {
  std::size_t operator()(const Vect2D &p) const {
    using std::hash;

    return (hash<int>()(p.x) ^ (hash<int>()(p.y) << 1));
  }
};
}  //namespace std

namespace {
class SpaceMap {
 private:
  std::list<Vect2D> m_asteroids;
  size_t m_height;
  size_t m_width;

 public:
  SpaceMap(const std::vector<std::string> &input) {
    m_height = input.size();
    if (m_height == 0)
      return;

    m_width = input[0].length();

    for (size_t y{0}; y < m_height; ++y) {
      assert(input[y].length() == m_width);
      for (size_t x{0}; x < m_width; ++x) {
        if (input[y][x] == '#') {
          m_asteroids.emplace_back(x, y);
        }
      }
    }
  }

  int getMaxVisible() {
    int max{0};

    for (auto &ast : m_asteroids) {
      int visible = countVisible(ast);
      max = std::max(max, visible);
    }
    return max;
  }

  Vect2D getOptimalBase() {
    int max{0};
    Vect2D optimal;

    for (auto &ast : m_asteroids) {
      int visible = countVisible(ast);
      if (visible > max) {
        max = visible;
        optimal = ast;
      }
    }
    return optimal;
  }

  Vect2D vaporize(Vect2D base, int count) {
    Vect2D result;
    auto relMap = getRelative(base);
    auto mapIt = relMap.begin();

    for (int step{0}; step < count; ++step) {
      if (relMap.empty()) break;
      result = mapIt->second.front();
      mapIt->second.pop_front();

      if (mapIt->second.empty()) {
        mapIt = relMap.erase(mapIt);
      } else {
        ++mapIt;
      }

      if (mapIt == relMap.end())
        mapIt = relMap.begin();
    }

    return result;
  }

 private:
  int countVisible(const Vect2D &origin) {
    std::unordered_set<Vect2D> slopes;
    for (auto &ast : m_asteroids) {
      if (ast == origin)
        continue;
      slopes.insert(origin.calcSlope(ast));
    }
    return slopes.size();
  }

  std::map<Vect2D, std::list<Vect2D>, Vect2D_compAngle> getRelative(const Vect2D &origin) {
    std::map<Vect2D, std::list<Vect2D>, Vect2D_compAngle> result;

    for (auto &ast : m_asteroids) {
      if (ast == origin) continue;
      Vect2D slope = origin.calcSlope(ast);
      Vect2D dist = ast - origin;
      std::list<Vect2D> &astList = result[slope];

      auto it = astList.begin();
      while (it != astList.end()) {
        if (dist.lessMag(*it - origin))
          break;

        ++it;
      }
      astList.insert(it, ast);
    }
    return result;
  }
};

}  //namespace

int day10_1(const Stringlist &slist) {
  SpaceMap smap(slist.get());
  return smap.getMaxVisible();
}

int day10_2(const Stringlist &slist, int num) {
  SpaceMap smap(slist.get());
  Vect2D base = smap.getOptimalBase();
  Vect2D ast = smap.vaporize(base, num);
  return 100 * ast.x + ast.y;
}

void day10(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      slist.fromString(
          ".#..#\n"
          ".....\n"
          "#####\n"
          "....#\n"
          "...##");
      runTest(day10_1(slist), 8);
      slist.fromString(
          "......#.#.\n"
          "#..#.#....\n"
          "..#######.\n"
          ".#.#.###..\n"
          ".#..#.....\n"
          "..#....#.#\n"
          "#..#....#.\n"
          ".##.#..###\n"
          "##...#..#.\n"
          ".#....####");
      runTest(day10_1(slist), 33);
      slist.fromString(
          "#.#...#.#.\n"
          ".###....#.\n"
          ".#....#...\n"
          "##.#.#.#.#\n"
          "....#.#.#.\n"
          ".##..###.#\n"
          "..#...##..\n"
          "..##....##\n"
          "......#...\n"
          ".####.###.");
      runTest(day10_1(slist), 35);
      slist.fromString(
          ".#..#..###\n"
          "####.###.#\n"
          "....###.#.\n"
          "..###.##.#\n"
          "##.##.#.#.\n"
          "....###..#\n"
          "..#.#..#.#\n"
          "#..#.#.###\n"
          ".##...##.#\n"
          ".....#.#..");
      runTest(day10_1(slist), 41);
      slist.fromString(
          ".#..##.###...#######\n"
          "##.############..##.\n"
          ".#.######.########.#\n"
          ".###.#######.####.#.\n"
          "#####.##.#.##.###.##\n"
          "..#####..#.#########\n"
          "####################\n"
          "#.####....###.#.#.##\n"
          "##.#################\n"
          "#####.##.###..####..\n"
          "..######..##.#######\n"
          "####.##.####...##..#\n"
          ".#####..#.######.###\n"
          "##...#.##########...\n"
          "#.##########.#######\n"
          ".####.#.###.###.#.##\n"
          "....##.##.###..#####\n"
          ".#.#.###########.###\n"
          "#.#.#.#####.####.###\n"
          "###.##.####.##.#..##");
      runTest(day10_1(slist), 210);

    } else {
      slist.fromString(
          ".#..##.###...#######\n"
          "##.############..##.\n"
          ".#.######.########.#\n"
          ".###.#######.####.#.\n"
          "#####.##.#.##.###.##\n"
          "..#####..#.#########\n"
          "####################\n"
          "#.####....###.#.#.##\n"
          "##.#################\n"
          "#####.##.###..####..\n"
          "..######..##.#######\n"
          "####.##.####...##..#\n"
          ".#####..#.######.###\n"
          "##...#.##########...\n"
          "#.##########.#######\n"
          ".####.#.###.###.#.##\n"
          "....##.##.###..#####\n"
          ".#.#.###########.###\n"
          "#.#.#.#####.####.###\n"
          "###.##.####.##.#..##");
      runTest(day10_2(slist, 1), 1112);
      runTest(day10_2(slist, 2), 1201);
      runTest(day10_2(slist, 3), 1202);
      runTest(day10_2(slist, 10), 1208);
      runTest(day10_2(slist, 50), 1609);
      runTest(day10_2(slist, 200), 802);
    }

  } else {
    if (slist.fromFile(filename)) {
      int result;

      result = (part == 1) ? day10_1(slist) : day10_2(slist, 200);
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}