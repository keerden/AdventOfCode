#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <set>
#include <list>
#include <utility>

using namespace std;

#define PREAMSIZE 25



int main()
{
  string line;
  ifstream file;
  list<pair<long long, set<long long>>> preamble;
  list<pair<long long, set<long long>>>::iterator it;

  long long num{}, count{0};

  file.open("input/day9.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(file >> num){
    cout << num << ":";
    if(count >= PREAMSIZE){
      //check
      for(it = preamble.begin(); it != preamble.end(); it++){
        //cout << " " << it->first;
        if(it->second.count(num)){
          cout << "found" << endl;
          break;
        }
      }

      if(it == preamble.end()){
        cout << endl << num << " is not a sum!" << endl;
        return 0;
      }

      //remove first
      preamble.pop_front();
    }

    count++;
    //add result to sets
    //cout << "adding " << num << " to:";
    for(it = preamble.begin(); it != preamble.end(); it++){
      it->second.insert(it->first + num);
    //  cout << it->first << "=" << it->first + num << " ";
    }
    //cout << endl;
    //add to list
    pair<long long, set<long long>> newpair;
    newpair.first = num;
    preamble.push_back(newpair);
  }

  return 0;
}