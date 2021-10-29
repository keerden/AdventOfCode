#include <bits/stdc++.h>

#include "solutions.h"

using solution_t = void (*)(int, bool, std::string);

std::vector<solution_t> solutions = {day1, day2, day3, day4, day5, day6, day7, day8, day9, day10,
                                     day11, day12, day13, day14, day15, day16, day17};

void printUsage(char *name) {
  std::cout << "Usage " << name << " daynr [[part = 1] [file = input/day[nr].txt]]\n";
  std::cout << "      " << name << " test daynr [part = 1]\n";
  std::cout << "Example: " << name << " 3 1 ../text.txt\n";
  std::cout << "         " << name << " test 5\n";
}
int main(int argc, char **argv) {
  int daynum{0};
  int part{1};
  bool test{false};
  std::string filename{""};

  if (argc < 2 || argc > 4) {
    printUsage(argv[0]);
    return 0;
  }

  if (argv[1] == std::string{"test"}) {
    if (argc < 3) {
      printUsage(argv[0]);
      return 0;
    }
    test = true;
    daynum = atoi(argv[2]);
    if (daynum < 1) {
      printUsage(argv[0]);
      return 0;
    }
    if (argc == 4) {
      part = atoi(argv[3]);
      if (part < 1 || part > 2) {
        printUsage(argv[0]);
        return 0;
      }
    }
  } else {
    daynum = atoi(argv[1]);
    if (daynum < 1) {
      printUsage(argv[0]);
      return 0;
    }
    if (argc > 2) {
      part = atoi(argv[2]);
      if (part < 1 || part > 2) {
        printUsage(argv[0]);
        return 0;
      }
      if (argc == 4) {
        filename = argv[3];
      }
    }
  }

  if (filename == "")
    filename = "input/day" + std::to_string(daynum) + ".txt";

  if (static_cast<size_t>(daynum) > solutions.size()) {
    std::cout << "Solution not found\n";
    return 0;
  }
  if (test) {
    std::cout << "Testing day " << daynum << ", Part " << part << "\n\n";
  } else {
    std::cout << "Running day " << daynum << ", Part " << part << " inputfile: " << filename << "\n\n";
  }

  solutions[daynum - 1](part, test, filename);

  return 0;
}