#include <bits/stdc++.h>

#include "solutions.h"

long long int  solution(const std::string &input, int n) {
  std::stringstream ss{input};
  long long int state[9]{0};
  std::string token;
  while (getline(ss, token, ',')) {
    int time = stoi(token);
    state[time]++;
  }

  for (int step{0}; step < n; ++step) {
    long long int  pos0 = state[0];
    for (int i{0}; i < 8; ++i) {
      state[i] = state[i + 1];
    }
    state[8] = pos0;
    state[6] += pos0;
  }

  long long int sum{0};
  for (int i{0}; i < 9; ++i) {
    sum += state[i];
  }

  return sum;
}

long long int  day6_1(const std::string &input) {
  return solution(input, 80);
}
long long int  day6_2(const std::string&input) {
  return solution(input, 256);;
}

void day6(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      runTest(day6_1("3,4,3,1,2"), 5934);
    } else {
      runTest(day6_2("3,4,3,1,2"), 26984457539);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day6_1(slist.get(0)) << "\n";
      } else {
        std::cout << "result: " << day6_2(slist.get(0)) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}