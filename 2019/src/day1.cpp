#include <bits/stdc++.h>

#include "solutions.h"

int day1_1(const Stringlist &input) {
  int sum{0};

  for (auto &s : input) {
    sum += (stoi(s) / 3) - 2;
  }

  return sum;
}
int day1_2(const Stringlist &input) {
  int sum{0};
  int modfuel;

  for (auto &s : input) {
    modfuel = (stoi(s) / 3) - 2;

    do {
      sum += modfuel;
      modfuel = (modfuel / 3) - 2;
    } while (modfuel > 0);
  }

  return sum;
}

void day1(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      slist.fromString("12\n14\n1969\n100756");
      runTest(day1_1(slist), 34241);
    } else {
      slist.fromString("14\n1969\n100756");
      runTest(day1_2(slist), 2 + 966 + 50346);
    }
  } else {
    if (slist.fromFile(filename)) {
      int result;

      result = (part == 1) ? day1_1(slist) : day1_2(slist);
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}