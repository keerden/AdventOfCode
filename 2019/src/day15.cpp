#include <bits/stdc++.h>

#include "intcode.h"
#include "util.h"

namespace {

class Position {
 public:
  int x;
  int y;
  Position(int x, int y) : x{x}, y{y} {};
  Position(void) : x{0}, y{0} {};

  bool operator==(const Position &rhs) const {
    return (x == rhs.x) && (y == rhs.y);
  }
  friend std::ostream &operator<<(std::ostream &output, const Position &p) {
    output << "(" << p.x << ", " << p.y << ")";
    return output;
  }

  bool operator<(const Position &rhs) const {
    return (y < rhs.y) || ((y == rhs.y) && (x < rhs.x));
  }
};
}  //namespace

namespace std {

template <>
struct hash<Position> {
  std::size_t operator()(const Position &p) const {
    using std::hash;

    return (hash<int>()(p.x) ^ (hash<int>()(p.y) << 1));
  }
};
}  //namespace std

namespace {
class Robot {
 private:
  bool m_found;
  int m_top;
  int m_left;
  Position m_oxigen;

  Intcode m_computer;
  std::map<Position, int> m_map;  //0 = wall, 1 = empty 2 = oxygen

 public:
  Robot(const std::string &program) : m_found{false}, m_top{0}, m_left{0}, m_computer{program} {}

  void explore() {
    m_map[Position(0, 0)] = 1;
    explore(Position(0, 0));
  }

  void printMap() {
    int x = m_left;
    int y = m_top;

    for (auto &tile : m_map) {
      while (tile.first.y > y) {
        std::cout << "\n";
        x = m_left;
        ++y;
      }
      while (tile.first.x > x) {
        std::cout << " ";
        ++x;
      }
      if (x == 0 && y == 0)
        std::cout << "X";
      else
        std::cout << ((tile.second == 0) ? '#' : ((tile.second == 1) ? '.' : 'O'));

      ++x;
    }
    std::cout << "\n";
  }

  int spread(bool findOxygen) {
    std::queue<std::tuple<Position, int>> remaining;
    std::unordered_set<Position> visited;
    int maxDist{-1};

    if (findOxygen) {
      remaining.emplace(Position(0, 0), 0);
    } else {
      remaining.emplace(m_oxigen, 0);
    }

    while (!remaining.empty()) {
      auto tup = remaining.front();
      remaining.pop();
      Position pos = std::get<0>(tup);
      int dist = std::get<1>(tup);

      maxDist = std::max(maxDist, dist);

      visited.insert(pos);

      for (int dir = 1; dir <= 4; ++dir) {
        Position newpos{pos};
        switch (dir) {
          case 1:  //N
            --newpos.y;
            break;
          case 2:  //S
            ++newpos.y;
            break;
          case 3:  //W
            --newpos.x;
            break;
          case 4:  //E
            ++newpos.x;
            break;
        }

        if (visited.count(newpos) > 0)
          continue;

        if (m_map.count(newpos) != 0) {
          int value = m_map[newpos];

          if (value > 0) {
            if (value == 2 && findOxygen)
              return dist + 1;

            remaining.emplace(newpos, dist + 1);
          }
        }
      }
    }

    return (findOxygen ? -1 : maxDist);
  }

 private:
  void explore(Position pos) {
    for (int dir = 1; dir <= 4; ++dir) {
      int reverse;
      Position newpos{pos};
      switch (dir) {
        case 1:  //N
          --newpos.y;
          reverse = 2;
          break;
        case 2:  //S
          ++newpos.y;
          reverse = 1;
          break;
        case 3:  //W
          --newpos.x;
          reverse = 4;
          break;
        case 4:  //E
          ++newpos.x;
          reverse = 3;
          break;
      }

      m_top = std::min(m_top, newpos.y);
      m_left = std::min(m_left, newpos.x);

      //std::cout << "Pos " << pos << " dir: " << dir << " newpos: " << newpos;

      if (m_map.find(newpos) == m_map.end()) {
        int found = move(dir);
        m_map[newpos] = found;

        //std::cout << " new found: " << found << " map: " << m_map[newpos] << "\n";
        if (found != 0) {
          if (found == 2) {
            m_found = true;
            m_oxigen = newpos;
          }
          explore(newpos);
          move(reverse);
        }
      } else {
        //std::cout << " existing\n";
      }
    }
  }

  int move(int dir) {
    m_computer.setInput(dir);
    m_computer.run();
    return m_computer.getOutput();
  }
};

}  //namespace

int day15_1(const std::string &program) {
  Robot r(program);
  r.explore();
  r.printMap();

  return r.spread(true);
}

int day15_2(const std::string &program) {
  Robot r(program);
  r.explore();
  return r.spread(false);
}

void day15(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
    } else {
    }

  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day15_1(slist.get(0)) << "\n";

      } else {
        std::cout << "result: " << day15_2(slist.get(0)) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}