#include <bits/stdc++.h>

#include "solutions.h"

namespace {
class Board {
 private:
  std::vector<int> countX;
  std::vector<int> countY;
  size_t width;
  size_t height;
  std::map<int, Position2D> numbers;

 public:
  Board(const std::vector<int> &input) {
    height = 5;
    width = 5;
    for (size_t y{0}; y < height; ++y) {
      for (size_t x{0}; x < width; ++x) {
        Position2D p{(int)x, (int)y};
        numbers[input[x + width * y]] = p;
      }
    }
    for (size_t x{0}; x < width; ++x) countY.push_back(height);
    for (size_t y{0}; y < height; ++y) countX.push_back(width);
  }

  bool drawNumber(int num) {
    if (!numbers.contains(num)) {
      //std::cout << "b, not found " << num << "\n";
      return false;
    }

    Position2D pos = numbers[num];
    numbers.erase(num);
    //std::cout << "b, found " << num << " @ " << pos << "\n";
    return (--countX[pos.y] == 0) || (--countY[pos.x] == 0);
  }

  int calcScore() {
    int sum{0};
    for (auto &val : numbers) {
      sum += val.first;
    }
    return sum;
  }
};

}  //namespace

int day4_1(const Stringlist &input) {
  std::vector<Board> boards;
  std::vector<int> draw;
  std::string token;

  auto itt = input.begin();

  //parse line with draws
  std::stringstream ss;
  ss.str(*itt);
  while (getline(ss, token, ','))
    draw.push_back(std::stoi(token));

  ++itt;

  std::vector<int> numbers;
  int line = 0;
  while (itt != input.end()) {
    if (line > 0) {
      std::stringstream ss(*itt);
      for (int i{0}; i < 5; ++i) {
        int num;
        ss >> num;
        numbers.push_back(num);
      }
    }
    if (line == 5) {
      line = 0;
      boards.emplace_back(std::move(numbers));
      numbers.clear();
    } else {
      ++line;
    }
    ++itt;
  }

  int score{-1};
  for (auto &num : draw) {
    for (auto &b : boards) {
      //std::cout << "DRAW " << num << "\n";
      if (b.drawNumber(num)) {
        //std::cout << "BINGO\n";
        score = b.calcScore() * num;
        break;
      }
    }
    if (score > -1)
      break;
  }

  return score;
}
int day4_2(const Stringlist &input) {
  std::list<Board> boards;
  std::vector<int> draw;
  std::string token;

  auto itt = input.begin();

  //parse line with draws
  std::stringstream ss;
  ss.str(*itt);
  while (getline(ss, token, ','))
    draw.push_back(std::stoi(token));

  ++itt;

  std::vector<int> numbers;
  int line = 0;
  while (itt != input.end()) {
    if (line > 0) {
      std::stringstream ss(*itt);
      for (int i{0}; i < 5; ++i) {
        int num;
        ss >> num;
        numbers.push_back(num);
      }
    }
    if (line == 5) {
      line = 0;
      boards.emplace_back(std::move(numbers));
      numbers.clear();
    } else {
      ++line;
    }
    ++itt;
  }

  int score{-1};
  for (auto &num : draw) {
    auto itt = boards.begin();
    while (itt != boards.end()) {
      if (itt->drawNumber(num)) {
        if (boards.size() == 1) {
          score = itt->calcScore() * num;
          break;
        }

        itt = boards.erase(itt);
      }else{
        ++itt;
      }
    }
    if (score > -1)
      break;
  }

  return score;
}

void day4(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1\n"
        "\n"
        "22 13 17 11  0\n"
        " 8  2 23  4 24\n"
        "21  9 14 16  7\n"
        " 6 10  3 18  5\n"
        " 1 12 20 15 19\n"
        "\n"
        " 3 15  0  2 22\n"
        " 9 18 13 17  5\n"
        "19  8  7 25 23\n"
        "20 11 10 24  4\n"
        "14 21 16 12  6\n"
        "\n"
        "14 21 17 24  4\n"
        "10 16 15  9 19\n"
        "18  8 23 26 20\n"
        "22 11 13  6  5\n"
        " 2  0 12  3  7\n");
    if (part == 1) {
      runTest(day4_1(slist), 4512);
    } else {
      runTest(day4_2(slist), 1924);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day4_1(slist) << "\n";
      } else {
        std::cout << "result: " << day4_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}