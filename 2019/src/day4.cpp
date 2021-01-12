#include <bits/stdc++.h>

#include "util.h"

bool checknum(int i){
    std::string num = std::to_string(i);
    bool repeat{false};
    bool invalid{false};
    char c = num[0];

    if(num.length() != 6)
      return false;

    for (int j{1}; j < 6; ++j) {
      if (num[j] < c) {
        invalid = true;
        break;
      } else if (num[j] == c) {
        repeat = true;
      }
      c = num[j];
    }
    return (!invalid && repeat);
}

bool checknum2(int i) {
  std::string num = std::to_string(i);
  bool repeat{false};
  bool invalid{false};
  char c = num[0];
  int repeatcount = 0;
  for (int j{1}; j < 6; ++j) {
    if (num[j] < c) {
      invalid = true;
      break;
    } else if (num[j] == c) {
      ++repeatcount;
    } else {
      if (repeatcount == 1)
        repeat = true;
      repeatcount = 0;
    }
    c = num[j];
  }
  return (!invalid && (repeat || repeatcount == 1));
}

int day4_1(int min, int max) {
  int result{0};

  for (int i{min}; i <= max; ++i) {
    if(checknum(i))
      ++result;
  }
  return result;
}

int day4_2(int min, int max) {
  int result{0};

  for (int i{min}; i <= max; ++i) {
    if(checknum2(i))
      ++result;
  }
  return result;
}

void day4(int part, bool test, std::string filename) {
  if (test) {
    if (part == 1) {
      runTest(checknum(111111), true);
      runTest(checknum(223450), false);
      runTest(checknum(123789), false);
    } else {
      runTest(checknum2(112233), true);
      runTest(checknum2(123444), false);
      runTest(checknum2(111122), true);
      runTest(checknum2(123456), false);
      runTest(checknum2(123356), true);
    }
  } else {
    int result;

    result = (part == 1) ? day4_1(235741, 706948) : day4_2(235741, 706948);
    std::cout << "result: " << result << "\n";
  }
}
