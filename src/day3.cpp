#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
int main()
{
  string line;
  ifstream file;
  size_t pos = 0;
  int trees = 0;

  file.open("input/day3.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line.at(pos) == '#')
      trees++;
    pos += 3;
    if(pos >= line.length())
      pos -= line.length();
  }



  cout << "trees encountered: " << trees << endl;

  return 0;
}