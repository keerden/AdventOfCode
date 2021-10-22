#include <bits/stdc++.h>

#include "intcode.h"
#include "util.h"

ic_word_t day5_1(const Stringlist &input) {
  Intcode computer(input.get(0));
  computer.setAllInput({1});
  computer.run();

  auto output = computer.getAllOutput();
  // for(auto i: output){
  //   std::cout << i << "\n";
  // }

  return output.back();
}
ic_word_t day5_2(const std::string &program, ic_word_t input) {
  Intcode computer(program);
  computer.setAllInput({input});
  computer.run();
  return computer.getAllOutput().front();
}

void day5(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      Intcode computer("3,0,4,0,99");
      computer.setAllInput({42});
      computer.run();
      runTest(computer.getAllOutput() == std::list<ic_word_t>{42}, true);

      computer.reset("1002,4,3,4,33");
      computer.run();
      runTest(computer.memToString(), "1002,4,3,4,99");
    } else {
      runTest(day5_2("3,9,8,9,10,9,4,9,99,-1,8", 8), 1, "equal_to position mode, equal");
      runTest(day5_2("3,9,8,9,10,9,4,9,99,-1,8", 42), 0, "equal_to position mode, non equal");

      runTest(day5_2("3,9,7,9,10,9,4,9,99,-1,8", 1), 1, "less than position mode, less");
      runTest(day5_2("3,9,7,9,10,9,4,9,99,-1,8", 8), 0, "less than position mode, equal");
      runTest(day5_2("3,9,7,9,10,9,4,9,99,-1,8", 42), 0, "less than position mode, greater");

      runTest(day5_2("3,3,1108,-1,8,3,4,3,99", 8), 1, "equal_to immediate mode, equal");
      runTest(day5_2("3,3,1108,-1,8,3,4,3,99", 42), 0, "equal_to immediate mode, non equal");

      runTest(day5_2("3,3,1107,-1,8,3,4,3,99", 1), 1, "less than immediate mode, less");
      runTest(day5_2("3,3,1107,-1,8,3,4,3,99", 8), 0, "less than immediate mode, equal");
      runTest(day5_2("3,3,1107,-1,8,3,4,3,99", 42), 0, "less than immediate mode, greater");

      runTest(day5_2("3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9", 42), 1, "jump test, position mode, true");
      runTest(day5_2("3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9", 0), 0, "jump test, position mode, false");

      runTest(day5_2("3,3,1105,-1,9,1101,0,0,12,4,12,99,1", 42), 1, "jump test, immediate mode, true");
      runTest(day5_2("3,3,1105,-1,9,1101,0,0,12,4,12,99,1", 0), 0, "jump test, immediate mode, false");

      runTest(day5_2("3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99", 7), 999, "Test program, < 8");
      runTest(day5_2("3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99", 8), 1000, "Test program, == 8");
      runTest(day5_2("3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99", 9), 1001, "Test program, > 8");
    }
  } else {
    if (slist.fromFile(filename)) {
      ic_word_t result;

      result = (part == 1) ? day5_1(slist) : day5_2(slist.get(0), 5);
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}