#include <bits/stdc++.h>

#include "solutions.h"



int  day7_1(const std::string &input) {
  std::stringstream ss{input};
  std::string token;
  std::vector<int> crabs;
  int posmin = INT_MAX;
  int posmax = INT_MIN;

  while (getline(ss, token, ',')) {
    int pos = stoi(token);
    posmin = std::min(posmin, pos);
    posmax = std::max(posmax, pos);
    crabs.push_back(pos);
  }

  int summin{INT_MAX};
  for(int pos{posmin}; pos <= posmax; ++pos){
    int sum{0};
    for(auto &c:crabs){
      sum += abs(c - pos);
    }
    summin = std::min(sum, summin);
  }


  return summin;
}
long long int  day7_2(const std::string&input) {
  std::stringstream ss{input};
  std::string token;
  std::vector<int> crabs;
  int posmin = INT_MAX;
  int posmax = INT_MIN;

  while (getline(ss, token, ',')) {
    int pos = stoi(token);
    posmin = std::min(posmin, pos);
    posmax = std::max(posmax, pos);
    crabs.push_back(pos);
  }

  std::vector<int> cost;
  int lastcost{0};
  for(int dist{0}; dist <= (posmax - posmin); ++dist){
    int c = lastcost + dist;
    cost.push_back(c);
    lastcost = c;
  }

  int summin{INT_MAX};
  for(int pos{posmin}; pos <= posmax; ++pos){
    int sum{0};
    for(auto &c:crabs){
      sum += cost[abs(c - pos)];
    }
    summin = std::min(sum, summin);
  }


  return summin;
}

void day7(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      runTest(day7_1("16,1,2,0,4,2,7,1,2,14"), 37);
    } else {
      runTest(day7_2("16,1,2,0,4,2,7,1,2,14"), 168);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day7_1(slist.get(0)) << "\n";
      } else {
        std::cout << "result: " << day7_2(slist.get(0)) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}