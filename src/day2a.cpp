#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
int main()
{
  string line;
  ifstream file;
  int min, max;
  char c;
  char passwd[100];
  int valid = 0;

  file.open("input/day2.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    sscanf(line.c_str(), "%d-%d %c: %s", &min, &max, &c, passwd);
   
    if((passwd[min - 1] == c) != (passwd[max - 1] == c))
      valid++;
  }



  cout << "valid passwords: " << valid << endl;

  return 0;
}