#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <deque>

using namespace std;

typedef struct {
  int jolt;
  long long numsol;
} solution;

int main()
{
  string line;
  ifstream file;
  list<int> adapters;
  list<int>::iterator it;
  deque<solution> sol;
  deque<solution>::iterator sol_it;
  int num;
  long long result{0};

  
  file.open("input/day10.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(file >> num)
    adapters.push_back(num);
  adapters.sort();
  cout << "sorted";
  for(it = adapters.begin(); it != adapters.end(); it++)
    cout << *it << " ";
  
  cout << endl;

  sol.push_back({0,1});


  for(it = adapters.begin(); it != adapters.end(); it++){
    long long cur_result = 0;
    cout << "sol:" << *it;
    while(sol.size() && sol.begin()->jolt < *it - 3)
      sol.pop_front();

    for(sol_it = sol.begin(); sol_it != sol.end(); sol_it++){
      cout << " " << sol_it->jolt << "=" << sol_it->numsol;

      cur_result += sol_it->numsol;
    }
    sol.push_back({*it, cur_result});
    cout<< " total:" << cur_result << endl;

  }
  
  cout << "result:" << sol.rbegin()->numsol <<endl;


  return 0;
}