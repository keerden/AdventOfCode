#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>


using namespace std;

class intcode {
  public:
  vector<int> memory;
  size_t ip;


  intcode(vector<int> mem)
  : memory{mem}
  , ip{0}
  {}

  int get_value(int addr){
    return memory[addr];
  }

  void set_value(int addr, int value){
    memory[addr] = value;
  }

  void run() {
    int opcode, par1,par2, par3, isize{1};
    bool done{false};
    while(ip < memory.size() && !done){
        opcode = memory[ip];
        par1 = memory[ip+1];
        par2 = memory[ip+2];
        par3 = memory[ip+3];
        switch(opcode){
          case 1: //add
            set_value(par3, get_value(par1) + get_value(par2));
            isize = 4;
            break;
          case 2: //mul
            set_value(par3, get_value(par1) * get_value(par2));
            isize = 4;
            break;
          case 99: //halt
            done = true;
            isize = 1;
            break;
          default:
            cout << "error!\n";
            return;
            break;
        }   
        ip += isize;
    }
  }
};


int main()
{
  ifstream file;
  string line;
  vector<int> program;
  bool done{false};
  int noun, verb;

  file.open("input/day2.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file,line,','))
  {
    program.push_back(stoi(line));
  }

  for(noun = 0; noun < 100; noun++)
  {
    for(verb = 0; verb < 100; verb++){
      intcode computer(program);

      computer.set_value(1,noun);
      computer.set_value(2,verb);
      computer.run();
      if(computer.get_value(0) == 19690720){
        done = true;
        break;
      }
    }
    if(done)
      break;
  }

  cout << "noun " << noun << " verb " << verb << " result " << 100 * noun + verb << "\n";

  return 0;
}