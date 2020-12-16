#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>

using namespace std;

typedef struct {
  int from;
  int to;
} range;



void insert_range(list<range> *rlist, range r){
  for(list<range>::iterator it = rlist->begin(); it != rlist->end(); it++){
    if(it->from == r.from){
      if(it->to == r.to){
        return;
      }else if(it->to < r.to){
        rlist->insert(it, r);
        return;
      }
    } else if (it->from > r.from){
      rlist->insert(it, r);
      return;
    }
  }

  rlist->insert(rlist->end(), r);
}


int main()
{
  string line, snum, rule;
  int from1, to1, from2, to2, num;
  long long error{0};
  ifstream file;
  list<range> ranges;
  
  file.open("input/day16.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line == "")
      break;
    line = line.substr(line.find(": ") + 2);
    sscanf(line.c_str(), "%d-%d or %d-%d", &from1, &to1, &from2, &to2);
    insert_range(&ranges, {from1, to1});
    insert_range(&ranges, {from2, to2});
  }

  for(auto r:ranges)
    cout << "range [" << r.from << ", " << r.to << "]\n";


  while(getline(file, line)){ //ignore your ticket
    if(line == "")
      break;
  }

  while(getline(file, line)){ 
    if(line == "nearby tickets:")
      continue;
    stringstream ss(line);
    while(getline(ss, snum, ',')){
      num = stoi(snum);
      bool found{false};

      //check in range

      for(auto r:ranges){
        if(num >= r.from && num <= r.to){
          found = true;
          break;
        }
      }

      if(!found){
        error += num;
      }
    }
  }
  
  cout << "result: " << error << "\n";

  return 0;
}