#include <bits/stdc++.h>

#include "solutions.h"

namespace {
class RiskMap {
 protected:
  size_t m_width;
  size_t m_height;
  std::unordered_map<Position2D, char> m_risk;

 public:
  RiskMap() : m_width{0}, m_height{0} {}

  RiskMap(const Stringlist &input) {
    m_height = input.size();
    if (m_height == 0)
      return;

    m_width = input.get(0).size();

    for (size_t y{0}; y < m_height; ++y) {
      assert(input.get(y).size() == m_width);
      for (size_t x{0}; x < m_width; ++x) {
        m_risk[Position2D(x, y)] = input.get(y)[x] - '0';
      }
    }
  }

  long long int getRisk(void) {
    std::unordered_map<Position2D, long long int> totalRisk;
    std::unordered_set<Position2D> toVisit;
    totalRisk[Position2D(0, 0)] = 0;
    toVisit.insert(Position2D(0, 0));

    while (!toVisit.empty()) {
      std::unordered_set<Position2D> updated;
      for (auto &pos : toVisit) {
        long long int curRisk = totalRisk[pos];

        //update neighbours
        if ((size_t)pos.x > 0) {
          setCell(pos + Position2D(-1, 0), curRisk, totalRisk, updated);
        }
        if ((size_t)pos.y > 0) {
          setCell(pos + Position2D(0, -1), curRisk, totalRisk, updated);
        }
        if ((size_t)pos.x < m_width - 1) {
          setCell(pos + Position2D(1, 0), curRisk, totalRisk, updated);
        }
        if ((size_t)pos.y < m_height - 1) {
          setCell(pos + Position2D(0, 1), curRisk, totalRisk, updated);
        }
      }
      toVisit = std::move(updated);
    }

    // for (size_t y{0}; y < m_height; ++y) {
    //   for (size_t x{0}; x < m_width; ++x) {
    //     std::cout << totalRisk[Position2D(x, y)] << " ";
    //   }
    //   std::cout << "\n";
    // }

    return totalRisk[Position2D(m_width - 1, m_height - 1)];
  }

  void print(void) {
    for (size_t y{0}; y < m_height; ++y) {
      for (size_t x{0}; x < m_width; ++x) {
        std::cout << static_cast<int>(m_risk[Position2D(x, y)]);  // << " ";
      }
      std::cout << "\n";
    }
  }

 private:
  void setCell(Position2D pos,
               long long int curRisk,
               std::unordered_map<Position2D, long long int> &totalRisk,
               std::unordered_set<Position2D> &updated) {
    bool exist = totalRisk.count(pos) > 0;

    if (!exist || (totalRisk[pos] > (curRisk + m_risk[pos]))) {
      totalRisk[pos] = curRisk + m_risk[pos];
      updated.insert(pos);
    }
  }
};

class ExpandedRiskMap : public RiskMap {
 public:
  ExpandedRiskMap(const Stringlist &input) {
    size_t height = input.size();

    if (height == 0)
      return;

    size_t width = input.get(0).size();

    m_width = width * 5;
    m_height = height * 5;

    for (size_t y{0}; y < height; ++y) {
      assert(input.get(y).size() == width);
      for (size_t x{0}; x < width; ++x) {
        for (size_t xrep{0}; xrep < 5; ++xrep) {
          for (size_t yrep{0}; yrep < 5; ++yrep) {
            Position2D newpos(x + width * xrep, y + height * yrep);
            char val = input.get(y)[x] - '0' + xrep + yrep;
            m_risk[newpos] = val > 9 ? val - 9 : val;
          }
        }
      }
    }
  }
};
}  //namespace

int day15_1(const Stringlist &input) {
  RiskMap rm(input);
  return rm.getRisk();
}
int day15_2(const Stringlist &input) {
  ExpandedRiskMap rm(input);
  //rm.print();
  return rm.getRisk();
}

void day15(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "1163751742\n"
        "1381373672\n"
        "2136511328\n"
        "3694931569\n"
        "7463417111\n"
        "1319128137\n"
        "1359912421\n"
        "3125421639\n"
        "1293138521\n"
        "2311944581\n");
    if (part == 1) {
      runTest(day15_1(slist), 40);
    } else {
      runTest(day15_2(slist), 315);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day15_1(slist) << "\n";
      } else {
        std::cout << "result: " << day15_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}