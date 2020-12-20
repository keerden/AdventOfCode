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
  int m_id;
  std::vector<bool> m_edges[4];  // clockwise
  position m_pos;

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
  }
  int getId() { return m_id; }

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

  void findAdjacent(std::list<Tile *> &others, std::stack<Tile *> &assigned,
                    std::map<position, Tile *> &posmap) {
    auto it = others.begin();
    Tile *other;
    while (it != others.end() && !numAdjacent(posmap)) {
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

 private:
  bool numAdjacent(std::map<position, Tile *> &posmap) {
    for (size_t i{0}; i < 4; ++i) {
      if (posmap.count(calcPos(i)) == 0) return false;
    }

    return true;
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
        result = {m_pos.first, m_pos.second + 1};
        break;
      case Right:
        result = {m_pos.first + 1, m_pos.second};
        break;
      case Bottom:
        result = {m_pos.first, m_pos.second - 1};
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
  }

  void flip(bool horizontal) {
    std::vector<bool> edge;
    if (horizontal) {
      edge = m_edges[Right];
      m_edges[Right] = std::move(m_edges[Left]);
      m_edges[Left] = std::move(edge);
    } else {
      edge = m_edges[Top];
      m_edges[Top] = std::move(m_edges[Bottom]);
      m_edges[Bottom] = std::move(edge);
    }
    for (auto &edge : m_edges) {
      std::reverse(edge.begin(), edge.end());
    }
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

  result *= posmap[{min_x, min_y}]->getId();
  result *= posmap[{max_x, min_y}]->getId();
  result *= posmap[{min_x, max_y}]->getId();
  result *= posmap[{max_x, max_y}]->getId();

  std::cout << "result: " << result << "\n";

  return 0;
}