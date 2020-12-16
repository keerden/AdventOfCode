#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <set>
#include <algorithm>

using namespace std;
int main()
{
  string line;
  ifstream file;
  set<char>::iterator it;
  set<char> questions;
  set<char> intersect;
  set<char> tmp;
  int count =0;

  file.open("input/day6.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line == ""){
      count += intersect.size();
      cout << intersect.size() << endl;
      intersect.clear();
      questions.clear();
    }else{
      bool isFirst = questions.empty();
      questions.clear();
      for(int i = 0; i < line.size(); i++){
        questions.insert(line.at(i));
      }

      if(isFirst){
        intersect = questions;
      } else{
        tmp.clear();
        set_intersection(questions.begin(), questions.end(), intersect.begin(), intersect.end(), inserter(tmp, tmp.begin()));
        intersect = tmp;
      }


      cout << line <<" --set-- ";
      for(it = questions.begin(); it != questions.end(); it++)
        cout << *it;

      cout << " --intersect-- ";
      for(it = intersect.begin(); it != intersect.end(); it++)
        cout << *it;
      cout << endl;

    }
  }
  count += intersect.size();
  cout << intersect.size() << endl;


  cout << "total: " << count << endl;

  return 0;
}