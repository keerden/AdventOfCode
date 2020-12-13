#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef struct {
  int x;
  int y;
} direction;

int adj_occ(int orgX, int orgY, vector<string> &floorplan){
  
  direction dir[8] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
  int x, y;
  int result = 0;
  for(auto d: dir){
    x = orgX + d.x;
    y = orgY + d.y;

    while(x >= 0 && x < floorplan.size() && y >= 0 && y < floorplan[x].length()){
      if(floorplan[x][y] == '#')
        result++;
      if(floorplan[x][y] != '.')
        break;

      x+= d.x;
      y+= d.y;
    }
    
  }
  
  return result;
}

int main()
{
  string line;
  ifstream file;
  vector<string> prev;
  vector<string> cur;
  bool change = false;
  int occ = 0;
  
  file.open("input/day11.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line))
    cur.push_back(line);

  do{
    occ = 0;
    change = false;
    prev = cur;
    for(size_t x = 0; x < prev.size(); x++){
      for(size_t y = 0; y < prev[x].length(); y++){
        cout << prev[x][y];
        switch(prev[x][y])
        {
          case '#':
            if(adj_occ(x, y, prev) >=5){
              cur[x][y] = 'L';
              change = true;
            }
            occ++;
            break;
          case 'L':
            if(adj_occ(x, y, prev) == 0){
              cur[x][y] = '#';
              change = true;
            }
            break;
          default:
            break;
        }
      }
      cout << endl;
    }
    cout << occ << "seats occupied" << endl;
  }while(change);

  
  
  cout << "final occupied seats:" << occ << endl;


  return 0;
}