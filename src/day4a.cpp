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
  bool invalid = false;

  file.open("input/day4.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line == ""){
      // verify set
      if(!invalid){
        set_intersection(found.begin(), found.end(), required.begin(), required.end(), inserter(intersect, intersect.begin()));
        if(intersect == required){
          result++;
          cout << " -- OK" << endl;
        } else {
          cout << " -- FAIL" << endl;
        }
      } else {
        cout << " -- VALUE FAIL" << endl;
      }
      invalid = false;
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
        string value = line.substr(pos + 1, strt - pos - 2);
        cout << ":-" << value << "- ";

        if(key == "byr"){
          int num = stoi(value);
          if(num < 1920 || num > 2002 || value.size() != 4){
            invalid = true;
            cout << " !FAIL! ";
            break;
          }
        }
        if(key == "iyr"){
          int num = stoi(value);
          if(num < 2010 || num > 2020 || value.size() != 4){
            invalid = true;
            cout << " !FAIL! ";
            break;
          }
        }
        if(key == "eyr"){
          int num = stoi(value);
          if(num < 2020 || num > 2030 || value.size() != 4){
            invalid = true;
            cout << " !FAIL! ";
            break;
          }
        }
        if(key == "hgt"){
          size_t sz;
          int num = stoi(value, &sz);
          string hgt = value.substr(sz);
          if( (hgt == "in" && (num < 59 || num > 76))
              || (hgt == "cm" && (num < 150 || num > 193))
              || (hgt != "in" && hgt != "cm"))          
          {
            invalid = true;
            cout << " !FAIL! ";
            break;
          }
        }
        if(key == "hcl"){
          size_t i;
          for(i = 1; i < value.size(); i++){
            char c = value.at(i);
            if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')))
              break;
          }

          if(value.at(0) != '#' || i != 7 || value.size() != 7)         
          {
            invalid = true;
            cout << value.size() << " !FAIL!:" << i;
            break;
          }
        }

        if(key == "ecl"){
          if( value != "amb"  &&
              value != "blu"  &&
              value != "brn"  &&
              value != "gry"  &&
              value != "grn"  &&
              value != "hzl"  &&
              value != "oth"
          ){
            invalid = true;
            cout << " !FAIL! ";
            break;
          }
        }

        if(key == "pid"){
          size_t i;
          for(i = 1; i < value.size(); i++){
            char c = value.at(i);
            if(c < '0' && c > '9')
              break;
          }

          if(i != 9 || value.size() != 9)         
          {
            invalid = true;
            cout << value.size() << " !FAIL!:" << i;
            break;
          }
        }

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