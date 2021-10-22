#include <bits/stdc++.h>

#include "intcode.h"
#include "util.h"


ic_word_t day9_1(const std::string &program) {
  Intcode computer(program);
  computer.setAllInput({1});
  computer.run();
  return computer.getAllOutput().front();
}


size_t day9_2(const std::string &program) {
  Intcode computer(program);
  computer.setAllInput({2});
  computer.run();
  return computer.getAllOutput().front();
}

void day9(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
    Intcode computer("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99");
    computer.run();
    runTest(computer.outputToString(), "109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99");

    computer.reset("1102,34915192,34915192,7,4,7,99,0");
    computer.run();
    runTest(computer.outputToString().length(), (size_t) 16);

    computer.reset("104,1125899906842624,99");
    computer.run();
    runTest(computer.outputToString(), "1125899906842624");

    } else {

    }

  } else {
    if (slist.fromFile(filename)) {
      ic_word_t result;

      result = (part == 1) ? day9_1(slist.get(0)) : day9_2(slist.get(0));
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}