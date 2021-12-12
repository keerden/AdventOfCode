#include <bits/stdc++.h>

#include "solutions.h"

namespace {

class OctoMap {
 private:
  std::unordered_map<Position2D, int> oMap;
  size_t flashes;
  size_t width;
  size_t height;

 public:
  OctoMap() : flashes{0}, width{0}, height{0} {};
  OctoMap(const Stringlist &inp) : flashes{0} {
    height = inp.size();
    width = inp.get(0).size();

    for (size_t y{0}; y < height; ++y) {
      assert(inp.get(y).length() == width);
      for (size_t x{0}; x < width; ++x) {
        oMap.emplace(Position2D(x, y), inp.get(y)[x] - '0');
      }
    }
  }

  void printMap() {
    std::cout << toString() << "\n";
  }

  std::string toString() {
    std::stringstream ss{""};
    for (size_t y{0}; y < height; ++y) {
      for (size_t x{0}; x < width; ++x) {
        ss << oMap[Position2D(x, y)];
      }
      ss << "\n";
    }
    return ss.str();
  }

  size_t findSync(){
    int syncCount = oMap.size();
    size_t steps{1};
    while(step() != syncCount){
      ++steps;
    }
    return steps;
  }


  int step(size_t num) {
    int count{0};
    for (size_t i{0}; i < num; ++i)
      count += step();

    return count;
  }

  int step(void) {
    std::stack<Position2D> flashes;

    //inc by one, save flashes on stack
    for (auto &item : oMap) {
      ++item.second;
      if (item.second > 9) {
        flashes.push(item.first);
      }
    }

    //flash

    int flashCount{0};
    while (!flashes.empty()) {
      ++flashCount;
      Position2D fpos = flashes.top();
      flashes.pop();

      size_t x1 = fpos.x > 0 ? (fpos.x - 1) : (0);
      size_t x2 = std::min(fpos.x + 1, (int)width - 1);
      size_t y1 = fpos.y > 0 ? (fpos.y - 1) : (0);
      size_t y2 = std::min(fpos.y + 1, (int)height - 1);

      for (size_t y{y1}; y <= y2; ++y) {
        for (size_t x{x1}; x <= x2; ++x) {
          if ((size_t)fpos.x == x && (size_t)fpos.y == y)
            continue;

          Position2D fposnew(x, y);
          if (++oMap[fposnew] == 10) {
            flashes.push(fposnew);
          }
        }
      }
    }

    //reset flashed to 0
    for (auto &item : oMap) {
      if (item.second > 9)
        item.second = 0;
    }

    return flashCount;
  }
};

}  //namespace

int day11_1(const Stringlist &input) {
  OctoMap omap(input);
  return omap.step(100);
}
int day11_2(const Stringlist &input) {
  OctoMap omap(input);
  return omap.findSync();
}

void day11(int part, bool test, std::string filename) {
  Stringlist slist, slist2;
  if (test) {
    slist.fromString(
        "11111\n"
        "19991\n"
        "19191\n"
        "19991\n"
        "11111\n");

    slist2.fromString(
        "5483143223\n"
        "2745854711\n"
        "5264556173\n"
        "6141336146\n"
        "6357385478\n"
        "4167524645\n"
        "2176841721\n"
        "6882881134\n"
        "4846848554\n"
        "5283751526\n");
    if (part == 1) {
      OctoMap omap(slist);
      runTest(omap.step(2), 9);
      runTest(day11_1(slist2), 1656);
    } else {
      runTest(day11_2(slist2), 195);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day11_1(slist) << "\n";
      } else {
        std::cout << "result: " << day11_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}