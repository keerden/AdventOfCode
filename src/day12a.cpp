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
  //0 = N, 1 = E, 
  int posNS{0}, posEW{0};
  int wpNS{1}, wpEW{10};
  int NS;
 
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
        wpNS += amount;
      break;
      case 'E':
        wpEW += amount;
      break;
      case 'S':
        wpNS -= amount;
      break;
      case 'W':
        wpEW -= amount;
      break;
      case 'F':
        posNS += amount * wpNS;
        posEW += amount * wpEW;
      break;
      case 'L':  
        for(int i = 0; i < amount / 90; i++){
        NS = wpNS;
        wpNS = wpEW;
        wpEW = -NS;
        }
      break;
      case 'R':
      for(int i = 0; i < amount / 90; i++){
        NS = wpNS;
        wpNS = -wpEW;
        wpEW = NS;
      }
      break;
      default:
      break;
    }
    cout << " wpNS:" << wpNS << " wpEW:" << wpEW << " posNS:" << posNS<< " posEW:" << posEW << endl;
  }

  cout << "distance: N-S:" << abs(posNS) << " W-E:" << abs(posEW)<< " Total:" << abs(posNS) + abs(posEW) << endl;

  return 0;
}