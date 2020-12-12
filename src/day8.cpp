#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <utility>


typedef struct {
  enum {ACC, JMP, NOP} opcode;
  int argument;
  bool accessed;

} instruction;




using namespace std;
int main()
{
  string line;
  ifstream file;
  vector<instruction> program;
  int acc = 0, pc = 0;

  file.open("input/day8.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    instruction instr;
    stringstream ss(line);
    string op;

    ss >> op;
    ss >> instr.argument;
    if(op == "acc"){
      instr.opcode = instr.ACC;
    } else if(op == "jmp"){
      instr.opcode = instr.JMP;
    } else{
      instr.opcode = instr.NOP;
    }
    instr.accessed = false;
    program.push_back(instr);
  }

  do {
    instruction instr = program[pc];
    program[pc].accessed = true;
    switch(instr.opcode){
      case instr.JMP:
        pc += instr.argument;
        break;
      case instr.ACC:
        acc += instr.argument;
      case instr.NOP:
      default:
        pc++;
        break;
    }
  } while(pc < program.size() && !program[pc].accessed);

  cout << "final acc: " << acc << endl;

  return 0;
}