#include <bits/stdc++.h>

#include "solutions.h"

int day1_1(const Stringlist &input) {
  int count = 0;
  int biggest = 0;
  for(const std::string &line:input){
    if(line != ""){
      count += stoi(line);
      continue;
    }

    biggest = std::max(count, biggest);
    count = 0;
  }

  return biggest;
}
int day1_2(const Stringlist &input) {
  int count = 0;
  std::vector<int> elves;
  
  for(const std::string &line:input){
    if(line != ""){
      count += stoi(line);
      continue;
    }

    elves.push_back(count);
    count = 0;
  }

  if(count != 0){
    elves.push_back(count);
  }

  std::sort(elves.begin(), elves.end(), std::greater<int>());

  return elves[0] + elves[1] + elves[2];
}

void day1(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "1000\n" 
        "2000\n"
        "3000\n"
        "\n"
        "4000\n"
        "\n"
        "5000\n"
        "6000\n"
        "\n"
        "7000\n"
        "8000\n"
        "9000\n"
        "\n"
        "10000\n");
    if (part == 1) {
      runTest(day1_1(slist), 24000);
    } else {
      runTest(day1_2(slist), 45000);
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