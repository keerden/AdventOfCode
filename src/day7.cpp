#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>


using namespace std;

bool contains_bag(map<string, map<string, int>> ruleset, string color, string find, map<string, bool> *solutions){
  map<string, bool>::iterator solIt;
  map<string, int>::iterator it;

  solIt = solutions->find(color);
  if(solIt != solutions->end())  //solution already found
    return solIt->second;

  if(ruleset[color].count(find)){
    (*solutions)[color] = true;
    return true;
  }

  for(it=ruleset[color].begin(); it != ruleset[color].end(); it++){
    if(contains_bag(ruleset, it->first, find, solutions)){
      (*solutions)[color] = true;
      return true;
    }
  }
  (*solutions)[color] = false;
  return false;
}



int main()
{
  map<string, map<string, int>> ruleset;
  map<string, map<string, int>>::iterator it;
  string line;
  ifstream file;
  int result = 0;

  file.open("input/day7.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    vector<string> words;
    stringstream ss(line);
    string word, curBag;

    while(ss >> word){
      words.push_back(word);
    }
    
    curBag = words.at(0) + " " + words.at(1);
    if(words.at(4) != "no"){
      map<string, int> rule;
      for(size_t i = 4; i < words.size(); i+= 4){
        int num = stoi(words.at(i));
        ruleset[curBag][words.at(i+1) + " " + words.at(i+2)] = num;
      }
    }
  }

  map<string, bool> solutions;

  solutions["shiny gold"] = false;

  for(it=ruleset.begin(); it != ruleset.end(); it++){
    cout << "searching " << it->first << endl;
    if(it->first == "shiny gold")
      continue;

    if(contains_bag(ruleset, it->first, "shiny gold", &solutions))
      result++;
  }

  cout << "result: " << result << endl;

  return 0;
}