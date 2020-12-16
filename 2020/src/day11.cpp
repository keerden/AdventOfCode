#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int adj_occ(int x, int y, vector<string> *floorplan){
  int xmin = max(x - 1, 0);
  int xmax = min(x + 2, (int) floorplan->size());
  int ymin = max(y - 1, 0);
  int ymax = min(y + 2, (int) (*floorplan)[0].length());
  int result = 0;
  for(int i = xmin; i < xmax; i++){
    for(int j = ymin; j< ymax; j++){
      if((*floorplan)[i][j] == '#' && (i != x || j !=y))
        result++;
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
            if(adj_occ(x, y, &prev) >=4){
              cur[x][y] = 'L';
              change = true;
            }
            occ++;
            break;
          case 'L':
            if(adj_occ(x, y, &prev) == 0){
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