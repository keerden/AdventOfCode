#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>



using namespace std;
int main()
{
  string line;
  ifstream file;
  const char* str;
  int amount;
  //0 = N, 1 = E, 2 = S, 3 = W
  int pos[4]{0,0,0,0};
  int facing{1};

 
  file.open("input/day12.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line)){
    cout << line;
    str = line.c_str();
    amount = atoi(&str[1]);
    switch(str[0]){
      case 'N':
        pos[0] += amount;
      break;
      case 'E':
        pos[1] += amount;
      break;
      case 'S':
        pos[2] += amount;
      break;
      case 'W':
        pos[3] += amount;
      break;
      case 'F':
        pos[facing] += amount;
      break;
      case 'L':
        facing = (facing + 4 - (amount / 90)) % 4;
      break;
      case 'R':
        facing = (facing + (amount / 90)) % 4;
      break;
      default:
      break;
    }
    cout << " dir:" << facing << " pos N:" << pos[0] << " E:" << pos[1]<< " S:" << pos[2] << " W:" << pos[3] << endl;
  }


  cout << "total movement: N:" << pos[0] << " E:" << pos[1]<< " S:" << pos[2] << " W:" << pos[3] << endl;
  cout << "distance: N-S:" << abs(pos[0] - pos[2]) << " W-E:" << abs(pos[1] - pos[3])<< " Total:" << abs(pos[0] - pos[2]) + abs(pos[1] - pos[3]) << endl;

  return 0;
}