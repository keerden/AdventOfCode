#include <bits/stdc++.h>

#include "solutions.h"

int day2_1(const Stringlist &input) {
  int x{0}, y{0};

  for (auto &line : input) {
    std::stringstream ss{line};
    std::string dir;
    std::string token;
    int val;
    getline(ss, dir, ' ');
    getline(ss, token, ' ');
    val = stoi(token);

    if (dir == "forward") x += val;
    if (dir == "up") y -= val;
    if (dir == "down") y += val;
  }

  return x * y;
}
int day2_2(const Stringlist &input) {
  int x{0}, y{0}, aim{0};

  for (auto &line : input) {
    std::stringstream ss{line};
    std::string dir;
    std::string token;
    int val;
    getline(ss, dir, ' ');
    getline(ss, token, ' ');
    val = stoi(token);

    if (dir == "forward"){
       x += val;
       y += val * aim;
    }
    if (dir == "up") aim -= val;
    if (dir == "down") aim += val;
  }

  return x * y;
}

void day2(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "forward 5\n"
        "down 5\n"
        "forward 8\n"
        "up 3\n"
        "down 8\n"
        "forward 2\n");
    if (part == 1) {
      runTest(day2_1(slist), 150);
    } else {
      runTest(day2_2(slist), 900);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day2_1(slist) << "\n";
      } else {
        std::cout << "result: " << day2_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}