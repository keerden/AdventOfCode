#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>
#include <vector>
#include <map>
using namespace std;

typedef struct {
  int x;
  int y;
} position;

typedef struct {
  position from;
  position to;
} section;



inline int dist(int x, int y){
  return abs(x)+ abs(y);
}

int main()
{
  ifstream file;
  string line, cmd;
  stringstream ss;
  map<int, vector<section>> horizontal;
  map<int, vector<section>> vertical;
  position pos {0,0}, newpos{};
  int min_dist{INT_MAX};
  int val;

  file.open("input/day3.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  getline(file, line);
  ss.str(line);
  while(getline(ss, cmd, ',')){
    section sect;
    val = stoi(cmd.substr(1));
    switch(cmd[0]){
      case 'D':
        sect.to = pos;
        pos.y -= val;
        sect.from = pos;
        vertical[pos.x].push_back(sect);
        break;
      case 'U':
        sect.from = pos;
        pos.y += val;
        sect.to = pos;
        vertical[pos.x].push_back(sect);
        break;
      case 'L':
        sect.to = pos;
        pos.x -= val;
        sect.from = pos;
        horizontal[pos.y].push_back(sect);
        break;
      case 'R':
        sect.from = pos;
        pos.x += val;
        sect.to = pos;
        horizontal[pos.y].push_back(sect);
        break;
      default:
        break;
    }
  }

  cout << "horizontals: \n"; 

  for(auto h:horizontal){
    for(auto s:h.second)
      cout <<"\tfrom (" << s.from.x << ", " << s.from.y << ") to (" << s.to.x << ", " << s.to.y << ")\n";
  }

  cout << "verticals: \n"; 

  for(auto h:vertical){
    for(auto s:h.second)
      cout <<"\tfrom (" << s.from.x << ", " << s.from.y << ") to (" << s.to.x << ", " << s.to.y << ")\n";
  }



  pos = {0,0};
  getline(file, line);
  ss.clear();
  ss.str(line);
  while(getline(ss, cmd, ',')){
    val = stoi(cmd.substr(1));
    switch(cmd[0]){
      case 'U':{
        //check crospoints
        newpos = pos;
        newpos.y += val;
        auto it = horizontal.upper_bound(pos.y);
        bool cross = false;
        cout << 'U' << val << ":\n";
        while(!cross && it != horizontal.end() && it->first <= newpos.y && min_dist > dist(pos.x,it->first)){
          cout << "\thor " << it->first << "\n";
          for(auto sect: it->second){
            cout <<"\t\tfrom (" << sect.from.x << ", " << sect.from.y << ") to (" << sect.to.x << ", " << sect.to.y << ")\n";
            if(sect.from.x <= pos.x && sect.to.x >= pos.x){
              cross = true;
              min_dist = min(min_dist, dist(pos.x,it->first));
              break;
            }
          }
          it++;
        }
        pos = newpos;
        break;
      }
      case 'D':{
        //check crospoints
        newpos = pos;
        newpos.y -= val;
        auto it = horizontal.lower_bound(pos.y - 1);
        bool cross = false;
        cout << 'D' << val << ":\n";
        if(it != horizontal.end()){
          while(!cross && it != horizontal.begin() && it->first >= newpos.y && min_dist > dist(pos.x,it->first)){
            cout << "\thor " << it->first << "\n";
            for(auto sect: it->second){
              cout <<"\t\tfrom (" << sect.from.x << ", " << sect.from.y << ") to (" << sect.to.x << ", " << sect.to.y << ")\n";
              if(sect.from.x <= pos.x && sect.to.x >= pos.x){
                cross = true;
                min_dist = min(min_dist, dist(pos.x,it->first));
                break;
              }
            }
            it--;
          }
        }
        pos = newpos;
        break;
      }
      case 'R':{
        //check crospoints
        newpos = pos;
        newpos.x += val;
        auto it = vertical.upper_bound(pos.x);
        bool cross = false;
        cout << 'R' << val << ":\n";
        while(!cross && it != vertical.end() && it->first <= newpos.x && min_dist > dist(pos.y,it->first)){
          cout << "\tver " << it->first << "\n";
          for(auto sect: it->second){
            cout <<"\t\tfrom (" << sect.from.x << ", " << sect.from.y << ") to (" << sect.to.x << ", " << sect.to.y << ")\n";
            if(sect.from.y <= pos.y && sect.to.y >= pos.y){
              cross = true;
              min_dist = min(min_dist, dist(pos.y,it->first));
              break;
            }
          }
          it++;
        }
        pos = newpos;
        break;
      }
      case 'L':{
        //check crospoints
        newpos = pos;
        newpos.x -= val;
        auto it = vertical.lower_bound(pos.x - 1);
        bool cross = false;
        cout << 'L' << val << ":\n";
        if(it != vertical.end()){
          while(!cross && it != vertical.begin() && it->first >= newpos.x && min_dist > dist(pos.y,it->first)){
            cout << "\tver " << it->first << "\n";
            for(auto sect: it->second){
              cout <<"\t\tfrom (" << sect.from.x << ", " << sect.from.y << ") to (" << sect.to.x << ", " << sect.to.y << ")\n";
              if(sect.from.y <= pos.y && sect.to.y >= pos.y){
                cross = true;
                min_dist = min(min_dist, dist(pos.y,it->first));
                break;
              }
            }
            it--;
          }
        }
        pos = newpos;
        break;
      }
      default:
        break;
    }
  }


  cout << "min distance: " << min_dist << endl;

  return 0;
}