#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
  TilePosition operator+(const TilePosition rhs) {
    return TilePosition(x + rhs.x, y + rhs.y);
  }
  TilePosition operator-(const TilePosition rhs) {
    return TilePosition(x - rhs.x, y - rhs.y);
  }
  struct hash {
    size_t operator()(const TilePosition &p) const {
      return (std::hash<int>()(p.x)) ^ (std::hash<int>()(p.y));
    }
  };
};

using tileset = std::unordered_set<TilePosition, TilePosition::hash>;

class TileFloor {
 public:
  void setBlack(TilePosition pos) {
    auto it = black.find(pos);
    if (it == black.end()) {
      black.insert(pos);
    } else {
      black.erase(it);
    }
  }

  int getBlackCount() { return black.size(); }

 private:
  tileset black;
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

  std::cout << tfloor.getBlackCount() << " black tiles\n";

  return 0;
}