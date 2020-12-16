#include <iostream>
#include <fstream>
#include <string>
#include <cstring>


using namespace std;
int main()
{
  string line;
  ifstream file;
  int max  = 0;
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
    cout << "#" << count << " num " << num << endl;
    count++;
    if(num > max)
      max = num;
  }



  cout << "max: " << max << endl;

  return 0;
}