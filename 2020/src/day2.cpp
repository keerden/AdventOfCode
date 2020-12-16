#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
int main()
{
  string line;
  ifstream file;
  int min, max, count, len;
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
    len = strlen(passwd);
    cout << "min: " << min << " max " << max << " c: " << c << " pwd: " << passwd << " len " << len << endl;
    count = 0;
    for(int i = 0; i < len; i++){
      if(passwd[i] == c)
        count++;
    }

    cout << "count " << count << endl;
    
    if(count >= min && count <= max)
      valid++;
  }



  cout << "valid passwords: " << valid << endl;

  return 0;
}