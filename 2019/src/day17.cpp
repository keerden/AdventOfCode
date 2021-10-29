#include <bits/stdc++.h>

#include "intcode.h"
#include "util.h"

namespace {

class Routine {
 private:
  std::string m_path;

  struct Chunk {
    int sub;
    std::string_view path;
    Chunk() : sub{-1} {};
    Chunk(int sub) : sub{sub} {};
    Chunk(std::string_view path) : sub{-1}, path{path} {};
    Chunk(int sub, std::string_view path) : sub{sub}, path{path} {};
  };

 public:
  Routine(std::string path) : m_path{path} {};

  void printPath() {
    std::cout << m_path << "\n";
  }

  std::vector<std::string> findSubroutines() {
    std::vector<std::string> result(4);  //0 = mainroutine, 1 2 3 = A B C
    std::array<std::string_view, 3> subs;

    std::list<struct Chunk> chunkList;
    chunkList.emplace_front(-1, m_path);

    if (findSubs(0, chunkList, subs)) {
      bool first{true};

      for (auto &c : chunkList) {
        if (!first)
          result[0] += ",";

        first = false;
        result[0] += (c.sub + 'A');
      }

      result[1] = subs[0];
      result[2] = subs[1];
      result[3] = subs[2];
    }

    return result;
  }

 private:
  bool findSubs(size_t depth, std::list<struct Chunk> &chunkList, std::array<std::string_view, 3> &subResult) {
    auto it{chunkList.begin()};
    while (it != chunkList.end() && it->sub > -1) ++it;

    if (it == chunkList.end())
      return true;

    if (it->path.empty())
      return true;

    if (depth >= 3) {
      return false;
    }

    std::string_view curSub = maxSub(it->path);

    while (!curSub.empty()) {
      std::list<struct Chunk> chunks = chunkList;

      while (replaceSub(chunks, curSub, depth)) {
        if (chunks.size() > 10) {  //due to length of instruction string and comma's. Schrinking further won't work, so return
          return false;
        }

        if (findSubs(depth + 1, chunks, subResult)) {
          //found it!
          chunkList = std::move(chunks);
          subResult[depth] = curSub;
          return true;
        }
      }

      curSub = shrinkSub(curSub);
    }

    return false;
  }

  void printChunks(std::list<struct Chunk> &chunkList) {
    for (auto &c : chunkList) {
      if (c.sub == -1) {
        std::cout << "[" << c.path << "]";
      } else {
        std::cout << "[sub " << static_cast<char>(c.sub + 'A') << "]";
      }
    }
    std::cout << "\n";
  }

  bool replaceSub(std::list<struct Chunk> &chunks, std::string_view sub, size_t subNum) {
    auto it{chunks.begin()};
    while (it != chunks.end()) {
      if (it->sub == -1) {
        size_t pos = it->path.find(sub);
        if (pos != std::string::npos) {  //sub found
          //check if we need to split a chunk before
          if (pos > 0) {
            chunks.insert(it, Chunk(it->path.substr(0, pos - 1)));  // -1 to remove trailing comma
          }

          chunks.insert(it, Chunk(subNum));

          it->path = it->path.substr(pos + sub.size());
          if (it->path.empty()) {  // delete item
            chunks.erase(it);
          } else {
            it->path.remove_prefix(1);  //remove comma
          }

          return true;
        }
      }
      ++it;
    }
    return false;
  }

  std::string_view maxSub(const std::string_view &path) {
    std::string_view result = path.substr(0, 21);

    if (result.size() == 21) {  // too large, remove last comma
      result = result.substr(0, result.find_last_of(","));
    }
    return result;
  }

  std::string_view shrinkSub(const std::string_view &sub) {
    size_t pos = sub.find_last_of(",");

    if (pos == sub.npos)
      pos = 0;

    return sub.substr(0, pos);
  }
};

class RoboMap {
  Position2D m_robot;
  int m_direction;
  std::vector<std::string> m_image;

 public:
  RoboMap() : m_robot{}, m_direction{0} {}
  RoboMap(const std::list<ic_word_t> &image) {
    int x{0}, y{-1};
    bool newline = true;
    for (auto &pixel : image) {
      if (newline) {
        m_image.emplace_back();
        newline = false;
        x = 0;
        ++y;
      }
      if (pixel == '\n') {
        newline = true;
      } else {
        m_image.back().push_back(static_cast<char>(pixel));

        switch (pixel) {
          case '^':
            m_direction = 0;
            m_robot = Position2D(x, y);
            break;
          case '>':
            m_direction = 1;
            m_robot = Position2D(x, y);
            break;
          case 'v':
            m_direction = 2;
            m_robot = Position2D(x, y);
            break;
          case '<':
            m_direction = 3;
            m_robot = Position2D(x, y);
            break;
        }
      }
      ++x;
    }

    std::cout << "robot: " << m_direction << " @ " << m_robot << "\n";
  }

  void printImage() {
    for (auto &s : m_image) {
      std::cout << s << "\n";
    }
  }

  int calibrate() {
    int result{0};

    for (size_t y{1}; y < m_image.size() - 1; ++y) {
      for (size_t x{1}; x < m_image[y].size() - 1; ++x) {
        if (m_image[y][x] == '#' && m_image[y][x - 1] == '#' && m_image[y][x + 1] == '#' && m_image[y - 1][x] == '#' && m_image[y + 1][x] == '#') {
          result += x * y;
        }
      }
    }
    return result;
  }

  Routine findPath() {
    std::string result{};

    Position2D pos{m_robot};
    int dir = m_direction;

    bool done{false};
    bool first{true};
    bool left{false};

    size_t steps = walkDir(pos, dir);
    if (steps > 0) {
      result += std::to_string(steps);
      first = false;
    }

    while (!done) {
      //turn left;
      left = true;
      dir = (dir > 0) ? dir - 1 : 3;
      steps = walkDir(pos, dir);  //turn left and try
      if (steps == 0) {
        left = false;
        dir = (dir + 2) % 4;
        steps = walkDir(pos, dir);  //go 'right' by reversing
        if (steps == 0) {
          done = true;
          break;
        }
      }

      if (!first)
        result += ",";

      first = false;
      result += (left ? "L," : "R,") + std::to_string(steps);
    }
    return Routine(result);
  }

 private:
  size_t walkDir(Position2D &pos, int dir) {
    switch (dir) {
      case 0:
        return walkN(pos);
        break;
      case 1:
        return walkE(pos);
        break;
      case 2:
        return walkS(pos);
        break;
      case 3:
        return walkW(pos);
        break;
    }

    return 0;
  }

  size_t walkN(Position2D &pos) {
    size_t result{0};
    for (int y{pos.y - 1}; y >= 0 && m_image[y][pos.x] == '#'; --y) {
      ++result;
    }
    pos.y -= result;
    return result;
  }

  size_t walkW(Position2D &pos) {
    size_t result{0};

    for (int x{pos.x - 1}; x >= 0 && m_image[pos.y][x] == '#'; --x) {
      ++result;
    }
    pos.x -= result;
    return result;
  }

  size_t walkE(Position2D &pos) {
    size_t result{0};
    for (int x{pos.x + 1}; x < (int)m_image[pos.y].size() && m_image[pos.y][x] == '#'; ++x) {
      ++result;
    }
    pos.x += result;
    return result;
  }

  size_t walkS(Position2D &pos) {
    size_t result{0};
    for (int y{pos.y + 1}; y < (int)m_image.size() && m_image[y][pos.x] == '#'; ++y) {
      ++result;
    }
    pos.y += result;
    return result;
  }
};
}  //namespace

int day17_1(const std::string &program) {
  Intcode computer(program);
  computer.run();
  RoboMap robo(computer.getAllOutput());
  robo.printImage();

  return robo.calibrate();
}

int day17_2(const std::string &program) {
  Intcode computer(program);
  computer.run();
  RoboMap robo(computer.getAllOutput());
  robo.printImage();
  Routine r = robo.findPath();
  r.printPath();
  auto lines = r.findSubroutines();

  computer.reset(program);
  computer.writeMem(0, 2);
  for (auto &l : lines) {
    for (int i : l){
      computer.setInput(i);
    }
    computer.setInput('\n');
  }
  computer.setInput('n');
  computer.setInput('\n');
  computer.run();


  std::cout << "\n";
  ic_word_t out;
  while(computer.hasOutput()){
    out = computer.getOutput();
     std::cout << static_cast<char>(out);
  }
   std::cout << "\n";


  return out;
}

void day17(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
    } else {
    }

  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day17_1(slist.get(0)) << "\n";

      } else {
        std::cout << "result: " << day17_2(slist.get(0)) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}
