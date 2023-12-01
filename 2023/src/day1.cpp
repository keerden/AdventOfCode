#include <bits/stdc++.h>

#include "solutions.h"

std::string digitStrings[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
};



int day1_1(const Stringlist &input) {
  int total = 0;
  for(const std::string &line:input){
    int first = -1;
    int last = -1;

    for(const char c:line){
        if(c >= '0' && c <= '9'){
            int digit = c - '0';
            if(first == -1){
                first = digit;
            }
            last = digit;
        }
    }
    int value = first * 10 + last;
    std::cout << value << std::endl;
    total += value;
  }

  return total;
}
int day1_2(const Stringlist &input) {
  int total = 0;
  for(const std::string &line:input){
    size_t firstIndex =  std::string::npos;
    size_t lastIndex =  std::string::npos;
    int first = 0;
    int last = 0;

    for(int i = 0; i < 9; i++){
        size_t pos = line.find(digitStrings[i]);
        if(pos == std::string::npos){
            continue;
        }

        if(firstIndex == std::string::npos || firstIndex > pos){
            firstIndex = pos;
            first = i + 1;
        }

        pos =  line.rfind(digitStrings[i]);

        assert(pos != std::string::npos);
        pos += digitStrings[i].length() -  1;

        if(lastIndex == std::string::npos || lastIndex < pos){
            lastIndex = pos;
            last = i + 1;
        }

    }

    for(size_t i = 0; i < line.length(); i++){
        char c = line[i];
        if(c >= '0' && c <= '9'){
            int digit = c - '0';
            if(firstIndex == std::string::npos || firstIndex > i){
                firstIndex = i;
                first = digit;
            }
            if(lastIndex == std::string::npos || lastIndex < i){
                lastIndex = i;
                last = digit;
            }
        }
    }
    int value = first * 10 + last;

    total += value;
  }

  return total;
}

void day1(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
        slist.fromString(
            "1abc2\n"
            "pqr3stu8vwx\n"
            "1b2c3d4e5f\n"
            "treb7uchet"
        );
      runTest(day1_1(slist), 142);
    } else {
        slist.fromString(
            "two1nine\n"
            "eightwothree\n"
            "abcone2threexyz\n"
            "xtwone3four\n"
            "4nineeightseven2\n"
            "zoneight234\n"
            "7pqrstsixteen"
        );
      runTest(day1_2(slist), 281);
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

