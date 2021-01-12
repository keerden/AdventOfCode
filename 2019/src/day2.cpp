#include <bits/stdc++.h>

#include "intcode.h"
#include "util.h"

int day2_1(const Stringlist &input) {
  Intcode computer(input.get(0));
  computer.writeMem(1, 12);
  computer.writeMem(2, 2);
  computer.run();

  return computer.readMem(0);
}
int day2_2(const Stringlist &input) {
  int noun, verb;
  bool done{false};

  for (noun = 0; noun < 100; noun++) {
    for (verb = 0; verb < 100; verb++) {
      Intcode computer(input.get(0));

      computer.writeMem(1, noun);
      computer.writeMem(2, verb);
      computer.run();
      if (computer.readMem(0) == 19690720) {
        done = true;
        break;
      }
    }
    if (done)
      break;
  }

  return 100 * noun + verb;
}

void day2(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    Intcode computer("1,9,10,3,2,3,11,0,99,30,40,50");
    computer.run();
    runTest(computer.memToString(), "3500,9,10,70,2,3,11,0,99,30,40,50");

    computer.reset("1,0,0,0,99");
    computer.run();
    runTest(computer.memToString(), "2,0,0,0,99");

    computer.reset("2,3,0,3,99");
    computer.run();
    runTest(computer.memToString(), "2,3,0,6,99");

    computer.reset("2,4,4,5,99,0");
    computer.run();
    runTest(computer.memToString(), "2,4,4,5,99,9801");

    computer.reset("1,1,1,4,99,5,6,0,99");
    computer.run();
    runTest(computer.memToString(), "30,1,1,4,2,5,6,0,99");

  } else {
    if (slist.fromFile(filename)) {
      int result;

      result = (part == 1) ? day2_1(slist) : day2_2(slist);
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}