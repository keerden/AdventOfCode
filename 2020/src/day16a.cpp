#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <set>

using namespace std;

typedef struct {
  int from;
  int to;
} range;


typedef struct {
  string name;
  range r1;
  range r2;
} field;

typedef set<field> fieldset;

inline bool operator<(const field& lhs, const field& rhs)
{
  return lhs.name < rhs.name;
}


bool isValidField(field f, int num){
  return (num >= f.r1.from && num <= f.r1.to) || (num >= f.r2.from && num <= f.r2.to);
}

void printSet(fieldset s){
    for(auto f:s)
      cout << " " << f.name;
    cout << "\n";
}

void printTicket(vector<fieldset> t){
  int i{0};
  for(auto s:t){
    cout << "\tpos " << i;
    printSet(s);
    ++i;
  }
}

int main()
{
  string line, snum, rule;
  int from1, to1, from2, to2, num;
  long long result{1};
  int tmp;
  ifstream file;
  vector<field> fields;
  vector<fieldset> my_ticket;
  vector<int> ticket_values;
  
  file.open("input/day16.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    if(line == "")
      break;
    int pos = line.find(": ");
    rule = line.substr(0, pos);
    line = line.substr(pos + 2);
    sscanf(line.c_str(), "%d-%d or %d-%d", &from1, &to1, &from2, &to2);
    fields.push_back({rule, {from1, to1}, {from2, to2}});
  }

  while(getline(file, line)){ //my ticket
    if(line == "")
      break;
    if(line == "your ticket:")
      continue;

    stringstream ss(line);
    while(getline(ss, snum, ',')){
      num = stoi(snum);
      ticket_values.push_back(num);
      fieldset possible;
      for(auto f:fields){
        if(isValidField(f, num))
          possible.insert(f);
      }
      my_ticket.push_back(possible);
    }
  }
  
  while(getline(file, line)){ 
    if(line == "nearby tickets:")
      continue;
    stringstream ss(line);
    vector<fieldset> ticket = my_ticket;
    size_t pos{0};
    bool invalid{false};
    while(getline(ss, snum, ',')){
      num = stoi(snum);
      auto it = ticket[pos].begin();
      while(it != ticket[pos].end()){
        field f = *it;
        ++it;
        if(!isValidField(f, num)){
          ticket[pos].erase(f);
        }
      }
      if(ticket[pos].size() == 0){
        invalid = true;
        break;
      }
      ++pos;
    }
    if(!invalid)
      my_ticket = ticket;
  }
  //printTicket(my_ticket);
  //solve
  bool solved{false};
  while(!solved){
    solved = true;
   // cout << "iterate\n";
   // printTicket(my_ticket);
    for(size_t i = 0; i < my_ticket.size(); ++i){
      if(my_ticket[i].size() != 1){
        solved = false;
      }else{
        field f = *(my_ticket[i].begin());
        for(size_t j = 0; j < my_ticket.size(); ++j){
          if(my_ticket[j].size() > 1)
            my_ticket[j].erase(f);
        }
      }
    }
  }
  cout << "solved:\n";
  printTicket(my_ticket);

  cout << "ticket:\n";
  for(int i = 0; i < my_ticket.size(); ++i){
    string name = (*my_ticket[i].begin()).name;
    cout << "\t" << name << "\t" << ticket_values[i] << "\n";
    if(name.find("departure ") == 0){
      result *= ticket_values[i];
    }
  }


  cout << "result: " << result << "\n";

  return 0;
}