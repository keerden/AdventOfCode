#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <set>
#include <list>
#include <utility>
#include <vector>

using namespace std;

#define PREAMSIZE 25

long long solution1(vector<long long> *numbers){
  list<pair<long long, set<long long>>> preamble;
  list<pair<long long, set<long long>>>::iterator it;
  long long num{}, count{0};

  for(int i = 0; i < numbers->size(); i++){
    num = numbers->at(i);
    if(count >= PREAMSIZE){
      //check
      for(it = preamble.begin(); it != preamble.end(); it++){
        if(it->second.count(num)){
          break;
        }
      }

      if(it == preamble.end()){
        return num;
      }

      //remove first
      preamble.pop_front();
    }

    count++;
    //add result to sets
    for(it = preamble.begin(); it != preamble.end(); it++){
      it->second.insert(it->first + num);
    }
    //add to list
    pair<long long, set<long long>> newpair;
    newpair.first = num;
    preamble.push_back(newpair);
  }
  return 0;
}

int main()
{
  string line;
  ifstream file;

  vector<long long> numbers;
  long long num;
  int iBegin {0}, iEnd{0};
  long long sum{0}, min{0}, max{0};

  file.open("input/day9.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(file >> num)
    numbers.push_back(num);

  num = solution1(&numbers);
  cout << "num " << num << endl;

  while(iBegin < numbers.size() && num != sum){
    if(sum < num && iEnd < numbers.size()){  //expand set to bottom when sum < num
      sum += numbers[iEnd];
      iEnd++;
      //cout <<"expand"<< endl;
    }else{//schrink set from top when sum > num
      sum -= numbers[iBegin];
      iBegin++;
      //cout <<"schrink"<< endl;
    }
  }

  if(num != sum){
    cout << "no region found!" << endl;
    return 0;
  }

  min = numbers[iBegin];
  for(int i = iBegin; i < iEnd; i++){
    cout << numbers[i] << " - " << min << endl;
    min = numbers[i] < min ? numbers[i] : min;
    max = numbers[i] > max ? numbers[i] : max;
  }

  cout << "min:" <<  min << " max:" << max << " result: "<< min + max << endl;


  return 0;
}