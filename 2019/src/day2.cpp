#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>


using namespace std;
int main()
{
  ifstream file;
  string line;
  vector<int> program;
  bool done{false};
  size_t pc{0};

  file.open("input/day2.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file,line,','))
  {
    program.push_back(stoi(line));
  }

  program[1] = 12;
  program[2] = 2;
  
  for(pc = 0; pc < program.size() && !done; pc += 4){
    // cout << "pc: " << pc << "\n";
    // for(auto i:program){
    //   cout << i << ",";
    // }
    // cout <<" \n";
    switch(program[pc]){
      case 1: //add
        program[program[pc + 3]] = program[program[pc+1]] + program[program[pc+2]];
        break;
      case 2: //mul
        program[program[pc + 3]] = program[program[pc+1]] * program[program[pc+2]];
        break;
      case 99: //halt
        done = true;
        cout << "99=done!\n";
        break;
      default:
        cout << "error!\n";
        done = true;
        break;
    }
  }

   for(auto i:program){
     cout << i << ",";
   }

  cout << "\nresult at pos 0: " << program[0] << endl;

  return 0;
}