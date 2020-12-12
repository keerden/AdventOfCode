#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>


using namespace std;


int main()
{
  string line;
  ifstream file;
  list<int> seats;
  list<int>::iterator it;
  int result  = 0;
  int count =1;

  file.open("input/day5.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    int num = 0;
    for(int i = 0; i < line.size(); i++){
      num <<=1;
      if(line.at(i) == 'B' || line.at(i) == 'R')
        num |= 1;
    }
    seats.push_back(num);
  }
  seats.sort();

  int prev = seats.front() - 1;
  for (it=seats.begin(); it!=seats.end(); it++){
    cout << *it << endl;
    if(*it > prev + 1)
      result = prev + 1;
    prev = *it;
  }

  cout << "result: " << result << endl;

  return 0;
}