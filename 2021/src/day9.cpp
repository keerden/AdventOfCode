#include <bits/stdc++.h>

#include "solutions.h"

using basin_t = std::unordered_set<Position2D>;














int day9_1(const Stringlist &input) {
  std::vector<std::vector<unsigned char>> map;
  int risksum{0};
  int height{0};
  int width = input.get(0).length();


  for(auto &l:input){
    assert(l.length() == width);
    std::vector<unsigned char> row;
    for(auto &c:l){
      row.emplace_back(c - '0');
    }
    map.push_back(std::move(row));
    ++height;
  }

  for(int x{0}; x < width; ++x){
    for(int y{0}; y < height; ++y){
      bool lowest =  (y == 0 || map[y-1][x] > map[y][x])
                  && (y == (height - 1) || map[y+1][x] > map[y][x])
                  && (x == 0 || map[y][x-1] > map[y][x])
                  && (x == (width - 1) || map[y][x + 1] > map[y][x]);

      if(lowest) risksum += map[y][x] + 1;
    }
  }

  return risksum;
}

bool isValidPos(int x, int y, int w, int h){
  return (x >= 0 && x < w && y >= 0 && y < h); 
}

bool isValidPos(Position2D pos, int w, int h){
  return isValidPos(pos.x, pos.y, w, h);
}

void pushNeighbours(std::stack<Position2D> &s, Position2D pos, int w, int h){
   if(isValidPos(pos.x -1 , pos.y, w, h))
      s.emplace(pos.x - 1, pos.y);
    if(isValidPos(pos.x + 1, pos.y, w, h))
      s.emplace(pos.x + 1, pos.y);
    if(isValidPos(pos.x, pos.y - 1, w, h))
      s.emplace(pos.x, pos.y - 1);
    if(isValidPos(pos.x, pos.y + 1, w, h))
      s.emplace(pos.x, pos.y + 1); 
}

bool basinComp(const basin_t &first, const basin_t &second){
  return first.size() > second.size();
}

int day9_2(const Stringlist &input) {
  std::unordered_map<Position2D, unsigned char> unallocated;
  std::list<basin_t> basins;
  std::stack<Position2D> to_check;

  size_t height{0};
  size_t width = input.get(0).length();
  
  for(size_t y{0} ; y < input.size(); ++y){
    assert(input.get(y).length() == width);
    ++height;
    for(size_t x{0}; x < width; ++x){
      unallocated.emplace(Position2D(x,y),input.get(y)[x] - '0');
    }
  }


  //move lowpoints into own basin
  for(size_t x{0}; x < width; ++x){
    for(size_t y{0}; y < height; ++y){
      unsigned char c = unallocated[Position2D(x,y)];
      bool lowest =  (y == 0 || unallocated[Position2D(x, y - 1)] > c)
                  && (y == (height - 1) || unallocated[Position2D(x, y + 1)] > c)
                  && (x == 0 || unallocated[Position2D(x - 1, y)] > c)
                  && (x == (width - 1) || unallocated[Position2D(x + 1, y)] > c);

      if(lowest){
        //construct and insert a set with a single point
        basin_t b{Position2D(x,y)};
        basins.push_back(std::move(b)); 
        unallocated.erase(Position2D(x,y));
      }
    }
  }
  
  for(auto &bas:basins){
    assert(bas.size() == 1);
    assert(to_check.empty());

    //load lowest point
    Position2D pos = *bas.begin();
    //push unallocated neighbours on stack

    pushNeighbours(to_check, pos, width, height);

    while(!to_check.empty()){
      Position2D pos = to_check.top() ;
      to_check.pop();
      //check if is part of basin
      auto itt = unallocated.find(pos);
      if(itt != unallocated.end()){
        if(itt->second < 9){
          bas.insert(pos);
          pushNeighbours(to_check, pos, width, height);
        }
        unallocated.erase(pos);
      }
    }
    //add basin size to result
  }

  int result{1};
  assert(basins.size() >= 3);
  basins.sort(basinComp);
  auto itt = basins.begin();
  for(int i{0}; itt != basins.end() && i < 3; i++, itt++){
    result *= itt->size();
  }

  return result;
}

void day9(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "2199943210\n"
        "3987894921\n"
        "9856789892\n"
        "8767896789\n"
        "9899965678\n");
    if (part == 1) {
      runTest(day9_1(slist), 15);
    } else {
      runTest(day9_2(slist), 1134);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day9_1(slist) << "\n";
      } else {
        std::cout << "result: " << day9_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}