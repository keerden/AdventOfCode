#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;



int main()
{
  string line;
  ifstream file;
  list<int> adapters;
  list<int>::iterator it;
  int num, prev{0};
  int diffs[4] {0,0,0,1};

  
  file.open("input/day10.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(file >> num)
    adapters.push_back(num);
  adapters.sort();

  for(it = adapters.begin(); it != adapters.end(); it++){
    if(*it - prev > 3){
      cerr << "dif to high";
      return 0;
    }

    diffs[*it - prev]++;
    prev = *it;
  }
  
  cout << "1-dif:" << diffs[1] << " 3-dif:" << diffs[3]<< " mul:" << diffs[1] * diffs[3] << endl;


  return 0;
}