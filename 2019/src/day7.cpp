#include <bits/stdc++.h>

#include "intcode.h"
#include "util.h"

int maxOutput(int input, const std::unordered_set<int> &phases, const std::string &program){
  int max{INT_MIN};

  for(auto &phase: phases){
    Intcode computer(program);
    computer.setAllInput({phase, input});
    computer.run();
    int output {computer.getAllOutput().front()};

    std::unordered_set<int> newset = phases;  
    newset.erase(phase);                
    if(newset.empty()){
      max = std::max(max, output);
    }else{
      max = std::max(max, maxOutput(output, newset, program));
    }
  }
   
  return max;
}

int runFeedbackConfig(const std::vector<int> &config, const std::string &program){
  std::vector<Intcode> amps;
  for(auto conf : config){
    amps.emplace_back(program);
    amps.back().setInput(conf);
  }

  int input{0}, output{0};
  bool done{false};

  do{
    for(auto &amp: amps){
      amp.setInput(input);
      if(amp.run())
        done = true;
      output = amp.getOutput();
      input = output;
    }
  }while(!done);

  return output;

}


int day7_1(const std::string &program) {
  int result;

  result = maxOutput(0, {0,1,2,3,4}, program);
  
  return result;
}
int day7_2(const std::string &program) {
  int max{INT_MIN};

  std::vector<int> phases{5,6,7,8,9};

  do{
    max = std::max(max, runFeedbackConfig(phases, program));
  }while(std::next_permutation(phases.begin(), phases.end()));

  return max;

}

void day7(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      runTest(day7_1("3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0"), 43210);
      runTest(day7_1("3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0"), 54321);
      runTest(day7_1("3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0"), 65210);
    } else {
      runTest(day7_2("3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5"), 139629729);
      runTest(day7_2("3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10"), 18216);
    }
  } else {
    if (slist.fromFile(filename)) {
      int result;

      result = (part == 1) ? day7_1(slist.get(0)) : day7_2(slist.get(0));
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}