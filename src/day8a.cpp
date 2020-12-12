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

bool is_loop(int pc, vector<instruction> *program, int &acc){
  
  while(pc < program->size()){
    instruction instr = program->at(pc);

    if(instr.accessed)
      return true;

    program->at(pc).accessed = true;

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

  }
  return false;
}



int main()
{
  string line;
  ifstream file;
  vector<instruction> program;
  bool done = false;
  int acc = 0, pc = 0;
  int accTmp;

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
    accTmp = acc;
    switch(instr.opcode){
      case instr.JMP: 
        if(!is_loop(pc + 1,&program, accTmp)) {//act as nop
          acc = accTmp;
          done = true;
          break;
        }
        pc += instr.argument;
        break;
      case instr.ACC:
        acc += instr.argument;
        pc++;
        break;
      case instr.NOP:
        if(!is_loop(pc + instr.argument,&program, accTmp)) {//act as jmp
          acc = accTmp;
          done = true;
          break;
        }
        pc++;
      default:
        break;
    }
  } while(pc < program.size() && !done);

  cout << "final acc: " << acc << endl;

  return 0;
}