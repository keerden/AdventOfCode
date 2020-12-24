#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>

struct position_s {
  int x;
  int y;
};

using position = position_s;
using tileset = std::set<position>;
using tilemap= std::map<position, int>;

bool operator<(const position lhs, const position rhs) {
  bool result{false};
  if (lhs.x == rhs.x) {
    result = lhs.y < rhs.y;
  } else {
    result = lhs.x < rhs.x;
  }
  return result;
}

position operator+(const position lhs, const position rhs) {
  position result;
  result.x = lhs.x + rhs.x;
  result.y = lhs.y + rhs.y;

  return result;
}

bool isBlack(position pos, const tileset &black) {
  return black.count(pos) > 0;
}

int numAdjacentBlack(position pos, const tileset &black) {
  position adjacent[] = {{2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1}, {1, 1}};
  int count{0};

  for (auto adj : adjacent) {
    if (isBlack(pos + adj, black)) ++count;
  }
  return count;
}

void addAdjacentWhite(position pos, tilemap &white, const tileset &black) {
  position adjacent[] = {{2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1}, {1, 1}};

  for (auto adj : adjacent) {
    position p = pos + adj;
    if (!isBlack(p, black)){
      auto it = white.find(p);
      if(it != white.end()){
        it->second++;
      }else{
        white[p] = 1;
      }
    }
  }
}

void printTiles(const tileset &tiles){
  for(auto &t: tiles)
    std::cout << "(" << t.x << ", " << t.y << "), ";
  std::cout << "\n";
}

void printTiles(const tilemap &tiles){
  for(auto &t: tiles)
    std::cout << "(" << t.first.x << ", " << t.first.y << ")=" << t.second << ", ";
  std::cout << "\n";
}

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

    auto it = black.find(tilepos);
    if (it == black.end()) {
      black.insert(tilepos);
    } else {
      black.erase(it);
    }
  }

  //printTiles(black);

  for (int i{0}; i < 100; ++i) {
    tilemap whites;
    tileset newblack;

 //   std::cout << "day " << i << "\n";
    for(auto &tile : black){
      addAdjacentWhite(tile, whites, black);
      int adjBlack = numAdjacentBlack(tile, black);
      if(adjBlack == 1 || adjBlack == 2)
        newblack.insert(tile);
    }
  //  std::cout << "newblack1 "; 
  //  printTiles(newblack);

    for(auto &tile: whites){
      if(tile.second == 2)
        newblack.insert(tile.first);
    }
    black = std::move(newblack);
    //std::cout << "day " << i + 1 << ": "<< black.size() << " black tiles\n";
  }

  std::cout << black.size() << " black tiles\n";

  return 0;
}