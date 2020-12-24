#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace {
class TilePosition {
 public:
  int x;
  int y;

  TilePosition(int posx, int posy) : x{posx}, y{posy} {}
  TilePosition() : TilePosition(0, 0) {}
  TilePosition(std::string directions) {
    x = 0;
    y = 0;
    size_t strIndex{0};

    while (strIndex < directions.length()) {
      switch (directions[strIndex]) {
        case 'e':
          x += 2;
          break;
        case 's':
          --y;
          if (directions[++strIndex] == 'e') {
            ++x;
          } else {
            --x;
          }
          break;
        case 'w':
          x -= 2;
          break;

        case 'n':
          ++y;
          if (directions[++strIndex] == 'e') {
            ++x;
          } else {
            --x;
          }
          break;
        default:
          std::cout << "unknown direction!, ignoring\n";
          break;
      }
      ++strIndex;
    }
  }

  bool operator<(const TilePosition rhs) {
    bool result{false};
    if (x == rhs.x) {
      result = y < rhs.y;
    } else {
      result = x < rhs.x;
    }
    return result;
  }
  bool operator==(const TilePosition &rhs) const {
    return (this->x == rhs.x && this->y == rhs.y);
  }
  bool operator!=(const TilePosition &rhs) const {
    return (this->x != rhs.x || this->y != rhs.y);
  }
  TilePosition operator+(const TilePosition rhs) const{
    return TilePosition(x + rhs.x, y + rhs.y);
  }
  TilePosition operator-(const TilePosition rhs) const{
    return TilePosition(x - rhs.x, y - rhs.y);
  }
  struct hash {
    size_t operator()(const TilePosition &p) const {
      return (std::hash<int>()(p.x)) ^ (std::hash<int>()(p.y));
    }
  };
};

using tileset = std::unordered_set<TilePosition, TilePosition::hash>;
using tilemap = std::unordered_map<TilePosition, int, TilePosition::hash>;

class TileFloor {
 public:
  void setBlack(TilePosition pos) {
    auto it = m_black.find(pos);
    if (it == m_black.end()) {
      m_black.insert(pos);
    } else {
      m_black.erase(it);
    }
  }

  int getBlackCount() { return m_black.size(); }

  void flipDay() {
    tilemap whites;
    tileset newblack;

    //   std::cout << "day " << i << "\n";
    for (auto &tile : m_black) {
      addAdjacentWhite(tile, whites);
      int adjBlack = numAdjacentBlack(tile);
      if (adjBlack == 1 || adjBlack == 2) newblack.insert(tile);
    }
    //  std::cout << "newblack1 ";
    //  printTiles(newblack);

    for (auto &tile : whites) {
      if (tile.second == 2) newblack.insert(tile.first);
    }
    m_black = std::move(newblack);
  }

  void flipDay(int days) {
    for (int i{0}; i < days; ++i) flipDay();
  }

 private:
  tileset m_black;

  bool isBlack(const TilePosition &pos) {
    return m_black.count(pos) > 0;
  }

  int numAdjacentBlack(const TilePosition &pos) {
    TilePosition adjacent[] = {{2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1}, {1, 1}};
    int count{0};

    for (auto adj : adjacent) {
      if (isBlack(pos + adj)) ++count;
    }
    return count;
  }

  void addAdjacentWhite(const TilePosition &pos, tilemap &white) {
    TilePosition adjacent[] = {{2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1}, {1, 1}};

    for (auto adj : adjacent) {
      TilePosition p = pos + adj;
      if (!isBlack(p)) {
        auto it = white.find(p);
        if (it != white.end()) {
          it->second++;
        } else {
          white[p] = 1;
        }
      }
    }
  }

  void printBlackTiles() {
    for (auto &t : m_black) std::cout << "(" << t.x << ", " << t.y << "), ";
    std::cout << "\n";
  }
};

}  // namespace

int main() {
  std::string line;
  std::ifstream file;
  TileFloor tfloor;

  file.open("input/day24.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  while (getline(file, line)) {
    tfloor.setBlack(TilePosition(line));
  }

  // printTiles(black);

  tfloor.flipDay(100);

  std::cout << tfloor.getBlackCount() << " black tiles\n";

  return 0;
}