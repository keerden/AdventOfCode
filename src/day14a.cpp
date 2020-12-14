#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <climits>
#include <map>
#include <bitset>

using namespace std;



int main()
{
  string line, token;
  ifstream file;
  unsigned long long sum{0};
  unsigned long long mask_set{0};
  unsigned long long mask_x{0};
 
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
      mask_set = mask_x =  0;
      for(char c : line.substr(7)){
        mask_set <<= 1;
        mask_x <<= 1;
        if(c == '1')
          mask_set |= 1;
        if(c == 'X'){
          mask_x |= 1;
        }
      }
    //  cout << "maskx " << bitset<36>(mask_x) << " mask_s " << bitset<36>(mask_set) << endl;
    }else{  //mem[]
      int pos = line.find("] = ");
      addr = stoull(line.substr(4, pos - 4));
      value = stoull(line.substr(pos + 4));

    //  cout << " addr: " << addr << "=" << bitset<36>(addr);

      addr &= ~mask_x;
      addr |= mask_set;

    //  cout << " masked: " << addr << "=" << bitset<36>(addr) << "  val:" << value << endl;

      memory[addr] = value;

      for(unsigned long long mask = mask_x; mask != 0; mask = (mask - 1)&mask_x){
   //     cout << "addr (masked): " << addr << "=" << bitset<36>(addr);
   //     cout << " xmask: " << mask << "=" << bitset<36>(mask);
   //     cout << " final: " << (addr | mask) << "=" << bitset<36>(addr | mask) << "  val:" << value << endl;
        memory[addr | mask] = value;
      }     
    }
  }
  
  for(auto val: memory)
    sum += val.second;

  cout << "result: " << sum << endl;


  return 0;
}