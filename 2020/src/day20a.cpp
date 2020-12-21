#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using position = std::pair<int, int>;

namespace {
class Tile {
 private:
  enum e_edge { Top, Right, Bottom, Left };
  position m_pos;
  int m_id;
  std::vector<bool> m_edges[4];  // clockwise
  std::vector<std::vector<bool>> m_payload;
  size_t m_payloadSize;

 public:
  Tile(std::vector<std::string> &input) : m_pos{0, 0} {
    assert(input.size() > 1);
    m_id = std::stoi(input[0].substr(5));
    size_t height = input.size();
    size_t width = input[1].size();
    assert(width == height - 1);

    for (size_t x{0}; x < width; ++x) {
      m_edges[Top].push_back(input[1][x] == '#');
      m_edges[Bottom].push_back(input[height - 1][width - x - 1] == '#');
    }

    for (size_t y{1}; y < height; ++y) {
      m_edges[Right].push_back(input[y][width - 1] == '#');
      m_edges[Left].push_back(input[height - y][0] == '#');
    }

    for (size_t y{2}; y < height - 1; ++y) {
      std::vector<bool> row;
      for (size_t x{1}; x < width - 1; ++x) {
        row.push_back(input[y][x] == '#');
      }
      m_payload.push_back(std::move(row));
    }
    m_payloadSize = width - 2;
  }

  void printEdges() {
    int i{0};
    for (auto &e : m_edges) {
      std::cout << i << ": ";
      for (auto b : e) std::cout << b;

      std::cout << " ";
      ++i;
    }
    std::cout << std::endl;
  }

  void printPayload() {
    std::cout << "Tile: " << m_id << "\n";
    for (auto &vb : m_payload) {
      for (auto b : vb) std::cout << (b ? "#" : ".");

      std::cout << "\n";
    }
  }

  void findAdjacent(std::list<Tile *> &others, std::stack<Tile *> &assigned,
                    std::map<position, Tile *> &posmap) {
    auto it = others.begin();
    Tile *other;
    if (numAdjacent(posmap) == 0) flip(false);

    while (it != others.end() && !(numAdjacent(posmap) == 4)) {
      auto otherIt = it;
      other = *it;
      ++it;
      bool found{false};

      for (size_t ownEdge{0}; ownEdge < 4 && !found; ++ownEdge) {
        if (posmap.count(calcPos(ownEdge)) > 0) continue;

        for (size_t otherEdge{0}; otherEdge < 4 && !found; ++otherEdge) {
          if (checkEdge(m_edges[ownEdge], other->m_edges[otherEdge])) {
            // rotate other to fit
            other->rotate(((ownEdge + 2) % 4) - otherEdge);
            other->m_pos = calcPos(ownEdge);
            posmap[other->m_pos] = other;
            assigned.push(other);
            others.erase(otherIt);
            found = true;

          } else if (checkEdgeReverse(m_edges[ownEdge],
                                      other->m_edges[otherEdge])) {
            // rotate and flip
            other->rotate(((ownEdge + 2) % 4) - otherEdge);
            other->flip(ownEdge == Top || ownEdge == Bottom);
            other->m_pos = calcPos(ownEdge);
            posmap[other->m_pos] = other;
            assigned.push(other);
            others.erase(otherIt);
            found = true;
          }
        }
      }
    }
  }

  position getPos() { return m_pos; }
  int getId() { return m_id; }
  std::vector<std::vector<bool>> getPayload() { return m_payload; };
  size_t getPayloadSize() { return m_payloadSize; };

 private:
  int numAdjacent(std::map<position, Tile *> &posmap) {
    int result{0};
    for (size_t i{0}; i < 4; ++i) {
      if (posmap.count(calcPos(i)) != 0) ++result;
    }

    return result;
  }

  bool checkEdge(const std::vector<bool> &first,
                 const std::vector<bool> &second) {
    assert(first.size() == second.size());
    size_t size = first.size();
    bool result{true};
    for (size_t i{0}; i < size && result; ++i) {
      result = first[i] == second[size - i - 1];
    }
    return result;
  }

  bool checkEdgeReverse(const std::vector<bool> &first,
                        const std::vector<bool> &second) {
    assert(first.size() == second.size());
    size_t size = first.size();
    bool result{true};
    for (size_t i{0}; i < size && result; ++i) {
      result = first[i] == second[i];
    }
    return result;
  }

  position calcPos(size_t edge) {
    position result{};
    switch (edge) {
      case Top:
        result = {m_pos.first, m_pos.second - 1};
        break;
      case Right:
        result = {m_pos.first + 1, m_pos.second};
        break;
      case Bottom:
        result = {m_pos.first, m_pos.second + 1};
        break;
      case Left:
        result = {m_pos.first - 1, m_pos.second};
    }
    return result;
  }

  void rotate(int times = 1) {
    bool left{false};

    if (times < 0) {
      times = -times;
      left = true;
    }

    if (times == 3) {
      times = 1;
      left = !left;
    }
    for (int i{0}; i < times; ++i) {
      std::vector<bool> edge;
      if (left) {
        edge = m_edges[0];
        for (int j{0}; j < 3; ++j) {
          m_edges[j] = std::move(m_edges[j + 1]);
        }
        m_edges[3] = std::move(edge);
      } else {
        edge = m_edges[3];
        for (int j{3}; j > 0; --j) {
          m_edges[j] = std::move(m_edges[j - 1]);
        }
        m_edges[0] = std::move(edge);
      }
    }
    rotatePayload(times, left);
  }

  void rotatePayload(int times, bool left) {
    if (times < 0) {
      left = !left;
      times = -times;
    }
    times %= 4;

    if (times == 0) return;

    if (times == 2) {
      for (auto &vb : m_payload) {
        std::reverse(vb.begin(), vb.end());
      }
      std::reverse(m_payload.begin(), m_payload.end());
    }

    if ((times == 1 && left) || (times == 3 && !left)) {  // rotate 1 left
      auto rotated = m_payload;
      for (size_t y{0}; y < m_payloadSize; ++y) {
        for (size_t x{0}; x < m_payloadSize; ++x) {
          rotated[y][x] = m_payload[x][m_payloadSize - y - 1];
        }
      }
      m_payload = std::move(rotated);
    }

    if ((times == 3 && left) || (times == 1 && !left)) {  // rotate 1 right
      auto rotated = m_payload;
      for (size_t y{0}; y < m_payloadSize; ++y) {
        for (size_t x{0}; x < m_payloadSize; ++x) {
          rotated[y][x] = m_payload[m_payloadSize - x - 1][y];
        }
      }
      m_payload = std::move(rotated);
    }
  }

  void flip(bool horizontal) {
    std::vector<bool> edge;
    if (horizontal) {
      edge = m_edges[Right];
      m_edges[Right] = std::move(m_edges[Left]);
      m_edges[Left] = std::move(edge);
      for (auto &vb : m_payload) std::reverse(vb.begin(), vb.end());
    } else {
      edge = m_edges[Top];
      m_edges[Top] = std::move(m_edges[Bottom]);
      m_edges[Bottom] = std::move(edge);
      std::reverse(m_payload.begin(), m_payload.end());
    }
    for (auto &edge : m_edges) {
      std::reverse(edge.begin(), edge.end());
    }
  }
};

class Image {
 private:
  size_t m_sizeX{};
  size_t m_sizeY{};
  std::vector<std::vector<bool>> m_img;

  static constexpr size_t monsterW = 20;
  static constexpr size_t monsterH = 3;

  bool monster[monsterH][monsterW]{
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
      {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1},
      {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0}};

 public:
  Image(size_t pSize, int xMin, int xMax, int yMin, int yMax,
        std::map<position, Tile *> &posmap) {
    m_sizeX = (xMax - xMin + 1) * pSize;
    m_sizeY = (yMax - yMin + 1) * pSize;
    m_img.resize(m_sizeY);
    for (int y{yMin}; y <= yMax; ++y) {
      for (int x{xMin}; x <= xMax; ++x) {
        Tile *t = posmap[{x, y}];
        auto payload = t->getPayload();
        int posY = ((y - yMin) * pSize);
        for (size_t i{0}; i < pSize; ++i) {
          for (size_t j{0}; j < pSize; ++j) {
            m_img[posY + i].push_back(payload[i][j]);
          }
        }
      }
    }
  }

  void print() {
    for (auto &vb : m_img) {
      for (bool b : vb) {
        std::cout << (b ? "#" : ".");
      }
      std::cout << "\n";
    }
  }

  size_t countSet() {
    size_t result{0};
    for (auto &vb : m_img) {
      for (bool b : vb) {
        if(b) ++result;
      }
    }
    return result;
  }

  void searchMonsters() {
    for (int i{0}; i < 4; ++i) {
      if (checkImage()) return;
      rotate();
    }
    flip();
    for (int i{0}; i < 4; ++i) {
      if (checkImage()) return;
      rotate();
    }
  }

 private:
  bool checkImage() {
    bool found{false};
    for (size_t y{0}; y < m_sizeY; ++y) {
      for (size_t x{0}; x < m_sizeX; ++x) {
        if (checkMonster(x, y, false)) {
          std::cout << "found monster at x:" << x << " y:" << y << "\n";
          found = true;
           checkMonster(x, y, true);
        }
      }
    }
    return found;
  }

  bool checkMonster(size_t x, size_t y, bool clear) {
     if (x + monsterW >= m_sizeX || y + monsterH >= m_sizeY) return false;

    for (size_t posY{0}; posY < monsterH; ++posY) {
      for (size_t posX{0}; posX < monsterW; ++posX) {

        if(monster[posY][posX]){
          if(!m_img[y + posY][x + posX])
            return false;
          m_img[y + posY][x + posX] = !clear;
        }
      }
    }
    return true;
  }

  void rotate() {
    std::cout << "rotate\n";
    std::vector<std::vector<bool>> rotated;
    rotated.resize(m_sizeX);
    for (size_t y{0}; y < m_sizeX; ++y) {
      for (size_t x{0}; x < m_sizeY; ++x) {
        rotated[y].push_back(m_img[x][m_sizeX - y - 1]);
      }
    }
    m_img = std::move(rotated);
    m_sizeX = m_sizeY;
    m_sizeY = m_img.size();
  }
  void flip() {
    std::cout << "flip\n";
    std::reverse(m_img.begin(), m_img.end());
  }
};

}  // namespace

int main() {
  std::string line;
  std::ifstream file;
  std::list<Tile *> tiles;

  long long result{1};

  file.open("input/day20.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }
  std::vector<std::string> strTile;
  while (getline(file, line)) {  // collect rules
    if (line == "") {
      Tile *t = new Tile(strTile);
      tiles.push_back(t);
      strTile.clear();
    } else {
      strTile.push_back(std::move(line));
    }
  }

  if (strTile.size()) {
    Tile *t = new Tile(strTile);
    tiles.push_back(t);
  }

  std::list<Tile *> leftover = tiles;
  std::stack<Tile *> assigned;
  std::map<position, Tile *> posmap;

  Tile *first = leftover.front();
  assigned.push(first);
  leftover.pop_front();
  posmap[first->getPos()] = first;

  while (!assigned.empty()) {
    Tile *t = assigned.top();
    assigned.pop();
    t->findAdjacent(leftover, assigned, posmap);
  }

  int min_x{0}, max_x{0}, min_y{0}, max_y{0};
  for (auto &it : posmap) {
    position p = it.first;
    min_x = std::min(min_x, p.first);
    max_x = std::max(max_x, p.first);
    min_y = std::min(min_y, p.second);
    max_y = std::max(max_y, p.second);
  }

  Image img(first->getPayloadSize(), min_x, max_x, min_y, max_y, posmap);
  img.print();

  std::cout << "\n\nsearching for monsters\n\n";
  img.searchMonsters();

  img.print();
  result = img.countSet();

  std::cout << "result: " << result << "\n";

  return 0;
}