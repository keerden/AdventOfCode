#include <bits/stdc++.h>

#include "solutions.h"


char getWrongItem(const std::string &line) {
  std::vector<char> left;
  std::vector<char> right;
  std::vector<char> intersect(1);

  size_t halfSize = line.length() / 2;
  for(size_t i{0}; i < halfSize; ++i){
    left.push_back(line[i]);
    right.push_back(line[halfSize + i]);
  }

  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), intersect.begin());

  return *intersect.begin();
}

char getIntersect(const std::string &line1, const std::string &line2, const std::string &line3) {
  std::unordered_set<char> sack1(line1.begin(), line1.end());
  std::unordered_set<char> sack2(line2.begin(), line2.end());

  for(char c:line3){
    if(!sack1.count(c))
      continue;
    if(!sack2.count(c))
      continue;
    return c;
  }

  return 0;
}

int getItemScore(char item){
  return (item <= 'Z') ? (item - 'A' + 27) : (item - 'a' + 1);
}



int day3_1(const Stringlist &input) {
  int count{0};
  for(auto line:input){
    count += getItemScore(getWrongItem(line));
  }
  return count;
}
int day3_2(const Stringlist &input) {
  int count{0};
  for(size_t i{2}; i < input.size(); i+= 3){
    count += getItemScore(getIntersect(input.get(i-2), input.get(i-1), input.get(i)));
  }
  return count;
}

void day3(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "vJrwpWtwJgWrhcsFMMfFFhFp\n"
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\n"
        "PmmdzqPrVvPwwTWBwg\n"
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\n"
        "ttgJtRGJQctTZtZT\n"
        "CrZsJsPPZsGzwwsLwLmpwMDw");
    if (part == 1) {
      runTest(day3_1(slist), 157);
    } else {
      runTest(day3_2(slist), 70);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day3_1(slist) << "\n";
      } else {
        std::cout << "result: " << day3_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}