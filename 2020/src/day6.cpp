#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <set>

using namespace std;
int main()
{
  string line;
  ifstream file;
  set <char> distinct;
  int count =0;

  file.open("input/day6.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line == ""){
      count += distinct.size();
      distinct.clear();
    }else{
      for(int i = 0; i < line.size(); i++){
        distinct.insert(line.at(i));
      }
    }
  }
  count += distinct.size();


  cout << "total: " << count << endl;

  return 0;
}