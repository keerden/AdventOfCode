#include <bits/stdc++.h>

#include "solutions.h"

int day1_1(const Stringlist &input) {
  int count = 0;
  for (size_t i{1}; i < input.size(); ++i) {
    if (stoi(input.get(i)) > stoi(input.get(i - 1))) {
      ++count;
    }
  }

  return count;
}
int day1_2(const Stringlist &input) {
  int count = 0;
  int oldsum;
  for (size_t i{2}; i < input.size(); ++i) {
    int sum = stoi(input.get(i-2)) + stoi(input.get(i-1)) + stoi(input.get(i));
    if (i > 2 && sum > oldsum) {
      ++count;
    }
    oldsum = sum;
  }

  return count;
}

void day1(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "199\n"
        "200\n"
        "208\n"
        "210\n"
        "200\n"
        "207\n"
        "240\n"
        "269\n"
        "260\n"
        "263\n");
    if (part == 1) {
      runTest(day1_1(slist), 7);
    } else {
      runTest(day1_2(slist), 5);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day1_1(slist) << "\n";
      } else {
        std::cout << "result: " << day1_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}