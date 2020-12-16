#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <climits>
#include <map>

using namespace std;



int main()
{
  string line, token;
  ifstream file;
  unsigned long long sum{0};
  unsigned long long mask_set{0};
  unsigned long long mask_clear{0};
 
  unsigned long long value;
  unsigned long long addr;
  map<unsigned long long , unsigned long long> memory;
    
  file.open("input/day14.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line.find("mask = ") == 0){
      mask_set = mask_clear = 0;
      for(char c : line.substr(7)){
        mask_set <<= 1;
        mask_clear <<= 1;
        if(c == '1')
          mask_set |= 1;
        if(c == '0'){
          mask_clear |= 1;
        }
      }
    }else{  //mem[]
      int pos = line.find("] = ");
      addr = stoull(line.substr(4, pos - 4));
      value = stoull(line.substr(pos + 4));

      value &= ~mask_clear;
      value |= mask_set;


      memory[addr] = value;     
    }
  }
  
  for(auto val: memory)
    sum += val.second;

  cout << "result: " << sum << endl;


  return 0;
}