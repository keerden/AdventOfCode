#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;
int main()
{
  string reqFields[7] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
  set <string> required(reqFields, reqFields + 7);
  set <string> found;
  set <string> intersect;
  string line;
  ifstream file;
  int result = 0;
  
  file.open("input/day4.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line == ""){
      // verify set
      set_intersection(found.begin(), found.end(), required.begin(), required.end(), inserter(intersect, intersect.begin()));
      if(intersect == required){
        result++;
        cout << " -- OK" << endl;
      } else {
        cout << " -- FAIL" << endl;
      }
      intersect.clear();
      found.clear();

    } else{
      //parse keys into set
      size_t strt = 0;
      while(strt != line.npos){
        size_t pos = line.find(":", strt);
        string key = line.substr(strt, pos - strt);
        cout << key << " ";
        found.insert(key);
        strt = line.find(" " , pos);
        if(strt != line.npos)
          strt++;
      }
    }

  }

  set_intersection(found.begin(), found.end(), required.begin(), required.end(), inserter(intersect, intersect.begin()));
  if(intersect == required){
    result++;
    cout << " -- OK" << endl;
  } else {
    cout << " -- FAIL" << endl;
  }
  cout << "answer: " << result << endl;

  return 0;
}