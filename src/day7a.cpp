#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>


using namespace std;

int count_bags(map<string, map<string, int>> ruleset, string color, map<string, int> *solutions){
  map<string, int>::iterator solIt;
  map<string, int>::iterator it;
  int result = 1;

  solIt = solutions->find(color);
   if(solIt != solutions->end()){  //solution already found
      cout << "Solution known!";
      return solIt->second;
   }

  for(it=ruleset[color].begin(); it != ruleset[color].end(); it++){
    cout << color << " contains " << it->second << " " << it->first << endl;
    result += count_bags(ruleset, it->first, solutions) * it->second;
    cout << "result: " << result << endl;
  }
  (*solutions)[color] = result;
  return result;
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

  map<string, int> solutions;

  result = count_bags(ruleset, "shiny gold", &solutions) - 1;

  cout << "result: " << result << endl;

  return 0;
}