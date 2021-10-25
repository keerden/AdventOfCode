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
class Arcade {
 private:
  std::unordered_map<Position, int> disp;
  int top;
  int bottom;
  int left;
  int right;
  int score;
  int ballX;
  int paddleX;
  int blocks;

 public:
  Arcade() : top{0}, bottom{0}, left{0}, right{0}, score{0}, ballX{0}, paddleX{0}, blocks{0} {}

  void setTile(int x, int y, int value) {
    if (x == -1 && y == 0) {
      score = value;
      return;
    }

    Position p(x, y);

    if (value != 2 && disp[p] == 2) --blocks;
    if (value == 2 && disp[p] != 2) ++blocks;

    disp[p] = value;

    left = std::min(left, x);
    right = std::max(right, x);
    top = std::min(top, y);
    bottom = std::max(bottom, y);

    if (value == 3) paddleX = x;
    if (value == 4) ballX = x;
  }

  void setTiles(std::list<ic_word_t> &list) {
    while (list.size() >= 3) {
      int x = list.front();
      list.pop_front();
      int y = list.front();
      list.pop_front();
      int val = list.front();
      list.pop_front();
      setTile(x, y, val);
    }
  }

  int getJoystick() {
    int pos{0};
    if (ballX < paddleX) pos = -1;
    if (ballX > paddleX) pos = 1;

    return pos;
  }

  int countBlocks() {
    return blocks;
  }

  int getScore() {
    return score;
  }

  void printDisp() {
    size_t h = static_cast<size_t>(bottom - top) + 1;
    size_t w = static_cast<size_t>(right - left) + 1;

    std::vector<std::string> buff;
    for (size_t i{0}; i < h; ++i)
      buff.emplace_back(w, ' ');

    for (auto &t : disp) {
      char c;
      switch (t.second) {
        case 0:
          c = ' ';
          break;
        case 1:
          c = '+';
          break;
        case 2:
          c = '#';
          break;
        case 3:
          c = '=';
          break;
        case 4:
          c = 'O';
          break;
      }
      buff[t.first.y - top][t.first.x - left] = c;
    }

    for (auto &line : buff) {
      std::cout << line << "\n";
    }
  }
};
}  //namespace

int day13_1(const std::string &program) {
  Arcade a;
  Intcode computer(program);
  computer.run();

  //std::cout << computer.outputToString();

  auto output = computer.getAllOutput();
  a.setTiles(output);
  a.printDisp();

  return a.countBlocks();
}

int day13_2(const std::string &program) {
  Arcade a;
  Intcode computer(program);
  computer.writeMem(0, 2);
  bool done = false;
  do {
    done = computer.run();
    auto output = computer.getAllOutput();
    a.setTiles(output);
    // if (a.countBlocks() == 0)
    //  break;
    computer.setInput(a.getJoystick());
    //a.printDisp();
    std::cout << "b: " << a.countBlocks() << " s: " << a.getScore() << "\n";
  } while (!done);

  //std::cout << computer.outputToString();

  return a.getScore();
}

void day13(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
    } else {
    }

  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day13_1(slist.get(0)) << "\n";

      } else {
        std::cout << "result: " << day13_2(slist.get(0)) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}