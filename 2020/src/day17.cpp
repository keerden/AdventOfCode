#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::vector;

#define NUM_CYCL 6

class Cubes {
 private:
  vector<vector<vector<bool>>> space;
  int orginX{0}, orginY{0}, orginZ{0};
  int sizeX{0}, sizeY{0}, sizeZ{0};

 public:
  Cubes(vector<std::string> &initial) {
    sizeZ = 1 + 2 * NUM_CYCL;
    sizeY = initial.size() + 2 * NUM_CYCL;
    sizeX = initial[0].length() + 2 * NUM_CYCL;
    orginX = NUM_CYCL;
    orginY = NUM_CYCL;
    orginZ = NUM_CYCL;

    space.resize(sizeZ);
    for (auto &vvb : space) {
      vvb.resize(sizeY);
      for (auto &vb : vvb) {
        vb.resize(sizeX, false);
      }
    }

    int y{orginY};
    for (auto line : initial) {
      for (int i = 0; i < line.length(); ++i) {
        space[orginZ][y][i + orginX] = (line[i] == '#');
      }
      ++y;
    }
  }

  void printSpace() {
    int z{0};
    for (auto &vvb : space) {
      std::cout << "Layer Z = " << z - orginZ << "\n";
      for (auto &vb : vvb) {
        std::cout << "   ";
        for (bool b : vb) std::cout << (b ? '#' : '.');

        std::cout << "\n";
      }
      ++z;
    }
  }

  void printActive() {
    for (int z = 0; z < sizeZ; ++z) {
      std::cout << "Layer Z = " << z - orginZ << "\n";
      for (int y = 0; y < sizeY; ++y) {
        std::cout << "   ";
        for (int x = 0; x < sizeX; ++x) {
          std::cout << checkNeighbours(x, y, z);
        }
        std::cout << "\n";
      }
    }
  }

  bool newState(bool curstate, int neighbours) {
    return curstate ? (neighbours == 2 || neighbours == 3) : (neighbours == 3);
  }

  int checkNeighbours(int posx, int posy, int posz) {
    int activeCount = 0;
    int x1 = std::max(0, posx - 1);
    int x2 = std::min(sizeX - 1, posx + 1);
    int y1 = std::max(0, posy - 1);
    int y2 = std::min(sizeY - 1, posy + 1);
    int z1 = std::max(0, posz - 1);
    int z2 = std::min(sizeZ - 1, posz + 1);

    for (int x = x1; x <= x2; ++x) {
      for (int y = y1; y <= y2; ++y) {
        for (int z = z1; z <= z2; ++z) {
          if (space[z][y][x] && !(x == posx && y == posy && z == posz))
            activeCount++;
        }
      }
    }
    return activeCount;
  }

  void cycle() {
    vector<vector<vector<bool>>> newspace = space;
    for (int z = 0; z < sizeZ; ++z) {
      for (int y = 0; y < sizeY; ++y) {
        for (int x = 0; x < sizeX; ++x) {
          newspace[z][y][x] =
              newState(space[z][y][x], checkNeighbours(x, y, z));
        }
      }
    }
    space = std::move(newspace);
  }

  int run() {
    for (int i{0}; i < 6; ++i) cycle();

    int result{0};
    for (auto &vvb : space)
      for (auto &vb : vvb)
        for (bool b : vb)
          if (b) result++;

    return result;
  }
};

int main() {
  std::string line;
  std::ifstream file;
  vector<std::string> initial;

  int result{};

  file.open("input/day17.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  while (getline(file, line)) {
    initial.push_back(line);
  }

  Cubes cubes(initial);
  result = cubes.run();

  std::cout << "result: " << result << "\n";

  return 0;
}