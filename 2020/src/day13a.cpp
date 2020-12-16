#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

typedef struct {
  int period;
  int offset;
} period_t;

int main()
{
  string line, token;
  ifstream file;
  bool done = false;
  int id, offset{0};
  unsigned long long timestamp{1};
  unsigned long long common_mul{1};

  vector<period_t> periods;

    
  file.open("input/day13.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  getline(file, line);  //ignore timestamp
  getline(file, line);

  stringstream ss(line);
  while(getline(ss, token, ',')){
    if(token != "x"){
      id = stoi(token);
      periods.push_back({id, offset});  
    }
    offset++;
  }

  for(auto p: periods){
    cout << "period: " << p.period << "+" << p.offset;
    while((timestamp  + p.offset) % p.period)
      timestamp += common_mul;
    common_mul *= p.period;
    cout << " time: " << timestamp << "  mul: " << common_mul << endl;
  }

 

  cout << "result time: " << timestamp  << endl;

  return 0;
}