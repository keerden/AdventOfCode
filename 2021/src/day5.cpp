#include <bits/stdc++.h>

#include "solutions.h"

void updatePosition(std::unordered_map<Position2D, int> &points, int x, int y) {
  Position2D pos{x, y};

  //std::cout << "Update " << pos << "\n";
  auto itt = points.find(pos);
  if (itt == points.end()) {
    points[pos] = 1;
  } else {
    itt->second++;
  }
}

int day5_1(const Stringlist &input) {
  std::regex reg{"^(\\d+),(\\d+) -\\> (\\d+),(\\d+)"};
  std::smatch sm;
  std::unordered_map<Position2D, int> points;

  for (auto &line : input) {
    int x1, x2, y1, y2;

    if (!std::regex_match(line, sm, reg)) {
      std::cout << "MATCH ERROR\n";
      return 0;
    }

    x1 = stoi(sm[1].str());
    y1 = stoi(sm[2].str());
    x2 = stoi(sm[3].str());
    y2 = stoi(sm[4].str());

    if (x1 == x2) {  //vertical
      for (int y{std::min(y1, y2)}; y <= std::max(y1, y2); ++y)
        updatePosition(points, x1, y);
    } else if (y1 == y2) {  //horizontal
      for (int x{std::min(x1, x2)}; x <= std::max(x1, x2); ++x)
        updatePosition(points, x, y1);
    }
  }

  int count{0};
  for (auto &p : points) {
    //std::cout << "P = " << p.first << " -> " << p.second << "\n";

    if (p.second > 1) count++;
  }

  return count;
}
int day5_2(const Stringlist &input) {
  std::regex reg{"^(\\d+),(\\d+) -\\> (\\d+),(\\d+)"};
  std::smatch sm;
  std::unordered_map<Position2D, int> points;

  for (auto &line : input) {
    int x1, x2, y1, y2;

    if (!std::regex_match(line, sm, reg)) {
      std::cout << "MATCH ERROR\n";
      return 0;
    }

    x1 = stoi(sm[1].str());
    y1 = stoi(sm[2].str());
    x2 = stoi(sm[3].str());
    y2 = stoi(sm[4].str());

    if (x1 == x2) {  //vertical
      for (int y{std::min(y1, y2)}; y <= std::max(y1, y2); ++y)
        updatePosition(points, x1, y);
    } else if (y1 == y2) {  //horizontal
      for (int x{std::min(x1, x2)}; x <= std::max(x1, x2); ++x)
        updatePosition(points, x, y1);
    } else {  //diagonal
      int deltaY = (y1 < y2) ? 1 : -1;
      if (x1 < x2) {
        for (int x{x1}, y{y1}; x <= x2; ++x, y += deltaY)
          updatePosition(points, x, y);
      } else {
        for (int x{x1}, y{y1}; x >= x2; --x, y += deltaY)
          updatePosition(points, x, y);
      }
    }
  }

  int count{0};
  for (auto &p : points) {
    //std::cout << "P = " << p.first << " -> " << p.second << "\n";

    if (p.second > 1) count++;
  }

  return count;
}

void day5(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "0,9 -> 5,9\n"
        "8,0 -> 0,8\n"
        "9,4 -> 3,4\n"
        "2,2 -> 2,1\n"
        "7,0 -> 7,4\n"
        "6,4 -> 2,0\n"
        "0,9 -> 2,9\n"
        "3,4 -> 1,4\n"
        "0,0 -> 8,8\n"
        "5,5 -> 8,2\n");
    if (part == 1) {
      runTest(day5_1(slist), 5);
    } else {
      runTest(day5_2(slist), 12);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day5_1(slist) << "\n";
      } else {
        std::cout << "result: " << day5_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}