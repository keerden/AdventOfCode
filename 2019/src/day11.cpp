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
  Position m_pos;
  int m_direction;  //0: up, 1: right, 2: down, 3: left;
  int m_left;
  int m_right;
  int m_top;
  int m_bottom;

  std::unordered_map<Position, bool> m_painted;

 public:
  Robot() : m_pos{0, 0}, m_direction{0}, m_left{0}, m_right{0}, m_top{0}, m_bottom{0} {}
  bool process(bool isWhite, bool turnRight) {
    bool result = false;
    m_painted[m_pos] = isWhite;
    m_direction += turnRight ? 1 : 3;
    m_direction %= 4;

    switch (m_direction) {
      case 0:
        --m_pos.y;
        break;
      case 1:
        ++m_pos.x;
        break;
      case 2:
        ++m_pos.y;
        break;
      case 3:
        --m_pos.x;
        break;
    }

    m_left = std::min(m_left, m_pos.x);
    m_right = std::max(m_right, m_pos.x);
    m_top = std::min(m_top, m_pos.y);
    m_bottom = std::max(m_bottom, m_pos.y);

    auto it = m_painted.find(m_pos);
    if (it != m_painted.end()) {
      result = it->second;
    }

    return result;
  }

  size_t paintCount() {
    return m_painted.size();
  }

  std::vector<std::string> getImage() {
    std::vector<std::string> result;

    size_t width = static_cast<size_t>(m_right - m_left + 1);
    size_t height = static_cast<size_t>(m_bottom - m_top + 1);

    //generate empty image
    for (size_t v{0}; v < height; ++v) {
      result.emplace_back(width, ' ');
    }

    for (auto &pos : m_painted) {
      size_t x = static_cast<size_t>(pos.first.x - m_left);
      size_t y = static_cast<size_t>(pos.first.y - m_top);

      result[y][x] = (pos.second ? '#' : ' ');
    }
    return result;
  }

  void printResult() {
    auto result = getImage();

    for(auto &line : result)
      std::cout << line << "\n";

  }

  void printMap() {
    for (auto &pos : m_painted) {
      std::cout << pos.first << " = " << ((pos.second == true) ? "White\n" : "Black\n");
    }
  }
};

}  //namespace

size_t day11_1(const std::string &program) {
  Intcode computer(program);
  computer.setAllInput({0});
  bool done = false;
  Robot r;

  do {
    done = computer.run();
    assert(computer.outputSize() >= 2);
    bool isWhite = computer.getOutput();
    bool turnRight = computer.getOutput();
    bool camera = r.process(isWhite, turnRight);

    computer.setInput(camera);

  } while (done == false);

  return r.paintCount();
}

void day11_2(const std::string &program) {
  Intcode computer(program);
  computer.setAllInput({1});
  bool done = false;
  Robot r;

  do {
    done = computer.run();
    assert(computer.outputSize() >= 2);
    bool isWhite = computer.getOutput();
    bool turnRight = computer.getOutput();
    bool camera = r.process(isWhite, turnRight);

    computer.setInput(camera);

  } while (done == false);

  r.printResult();
}

void day11(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      Robot r;
      runTest(r.process(1, 0), false);
      runTest(r.paintCount(), (size_t)1);
      r.printMap();

      runTest(r.process(0, 0), false);
      runTest(r.process(1, 0), false);
      runTest(r.process(1, 0), true);

      runTest(r.process(0, 1), false);
      runTest(r.process(1, 0), false);
      runTest(r.process(1, 0), false);
      runTest(r.paintCount(), (size_t)6);
    } else {
    }

  } else {
    if (slist.fromFile(filename)) {

      if(part == 1){
        std::cout << "result: " << day11_1(slist.get(0)) << "\n";

      }else{
        day11_2(slist.get(0));
      }
      

    } else {
      std::cout << "file not found!\n";
    }
  }
}