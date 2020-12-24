#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>


struct position_s {
  int x;
  int y;
};

bool operator< (const position_s lhs, const position_s rhs){
    bool result {false};
    if(lhs.x == rhs.x){
      result = lhs.y < rhs.y;
    } else{
      result = lhs.x < rhs.x;
    }
    return result;
  }

using position = position_s;
using tileset = std::set<position>;

int main() {
  std::string line;
  std::ifstream file;
  tileset black;
  int result{0};

  file.open("input/day24.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  while (getline(file, line)) {
    position tilepos = {0, 0};
    size_t strIndex{0};

    while (strIndex < line.length()) {
      switch (line[strIndex]) {
        case 'e':
          tilepos.x += 2;
          break;
        case 's':
          --tilepos.y;
          if (line[++strIndex] == 'e') {
            ++tilepos.x;
          } else {
            --tilepos.x;
          }
          break;
        case 'w':
          tilepos.x -= 2;
          break;

        case 'n':
          ++tilepos.y;
          if (line[++strIndex] == 'e') {
            ++tilepos.x;
          } else {
            --tilepos.x;
          }
          break;
        default:
          std::cout << "unknown direction!, ignoring\n";
          break;
      }
      ++strIndex;
    }

 

    //std::cout << "flip tile at position (" << tilepos.x << ", " << tilepos.y << ")";
    //check for position
    auto it = black.find(tilepos);
    if(it == black.end()){
      //std::cout << " to black\n";
      black.insert(tilepos);
    }else{
      //std::cout << " to white\n";
      black.erase(it);
    }
    
  }

  std::cout << black.size() << " black tiles\n";

  return 0;
}