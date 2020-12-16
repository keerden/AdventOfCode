#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <climits>

using namespace std;



int main()
{
  string line, token;
  ifstream file;
  int result, timestamp , ticks, id;
  int minId = 0;
  int minTicks = INT_MAX;
    
  file.open("input/day13.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  getline(file, line);
  timestamp = stoi(line);
  getline(file, line);

  stringstream ss(line);
  while(getline(ss, token, ',')){
    if(token != "x"){
      id = stoi(token);
      ticks = id - (timestamp % id );
      if(ticks < minTicks){
        minId = id;
        minTicks = ticks;
      }
      
    }
  }

  cout << "minutes to wait: " << minTicks << " bus ID:" << minId << " result: " << minId * minTicks << endl;


  return 0;
}