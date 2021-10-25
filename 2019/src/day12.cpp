#include <bits/stdc++.h>

#include "util.h"

namespace {

class Vect3D {
 public:
  int x;
  int y;
  int z;

  Vect3D(void) : x{0}, y{0}, z{0} {}
  Vect3D(int x, int y, int z) : x{x}, y{y}, z{z} {}
  Vect3D(const Vect3D &other) : x{other.x}, y{other.y}, z{other.z} {}
  Vect3D(const std::string &line) {
    std::regex e("^<x=(-?[\\d]+), y=(-?[\\d]+), z=(-?[\\d]+)>$");
    std::smatch sm;

    if (std::regex_match(line, sm, e) && sm.size() == 4) {
      x = stoi(sm[1]);
      y = stoi(sm[2]);
      z = stoi(sm[3]);
    }
  }

  Vect3D operator+(const Vect3D &rhs) const {
    return Vect3D(x + rhs.x, y + rhs.y, z + rhs.z);
  }

  Vect3D &operator=(const Vect3D &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  Vect3D operator-(const Vect3D &rhs) const {
    return Vect3D(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  friend std::ostream &operator<<(std::ostream &output, const Vect3D &p) {
    output << "<x=" << p.x << ", y=" << p.y << ", z=" << p.z << ">";
    return output;
  }

  int sumAbs() const {
    return abs(x) + abs(y) + abs(z);
  }
};

class Moon {
 public:
  Vect3D vel;
  Vect3D pos;

  Moon(const std::string &str) : vel{0, 0, 0}, pos{str} {}

  void applyGrav(Moon &other) {
    if (other.pos.x > pos.x) {
      ++vel.x;
      --other.vel.x;
    } else if (other.pos.x < pos.x) {
      --vel.x;
      ++other.vel.x;
    }
    if (other.pos.y > pos.y) {
      ++vel.y;
      --other.vel.y;
    } else if (other.pos.y < pos.y) {
      --vel.y;
      ++other.vel.y;
    }

    if (other.pos.z > pos.z) {
      ++vel.z;
      --other.vel.z;
    } else if (other.pos.z < pos.z) {
      --vel.z;
      ++other.vel.z;
    }
  }

  void applyVel() {
    pos = pos + vel;
  }

  void print() {
    std::cout << "pos=" << pos << ", vel=" << vel << "\n";
  }
};

long long int lcm(long long int a, long long int b) {
  long long int gcd = std::min(a, b);

  while (gcd > 1) {
    if ((a % gcd == 0) && (b % gcd == 0))
      break;
    --gcd;
  }

  return (a / gcd) * b;
}

}  //namespace

int day12_1(const Stringlist &slist, size_t numStep = 1) {
  std::vector<Moon> moons;
  for (auto &s : slist) moons.emplace_back(s);

  for (size_t step{0}; step < numStep; ++step) {
    //std::cout << "\nstep: " << step + 1 << "\n";
    for (size_t i{0}; i < moons.size(); ++i) {
      for (size_t j{i + 1}; j < moons.size(); ++j) {
        moons[i].applyGrav(moons[j]);
      }
      moons[i].applyVel();
      //moons[i].print();
    }
  }

  int energy{0};

  for (auto &m : moons) {
    energy += (m.pos.sumAbs() * m.vel.sumAbs());
  }

  return energy;
}

long long int day12_2(const Stringlist &slist) {
  std::vector<Moon> moons;
  for (auto &s : slist) moons.emplace_back(s);

  std::vector<Moon> initial = moons;

  long long int stepX{0};
  long long int stepY{0};
  long long int stepZ{0};
  long long int step{0};
  bool doneX{false}, doneY{false}, doneZ{false};

  while (!doneX || !doneY || !doneZ) {
    //std::cout << "\nstep: " << step + 1 << "\n";
    for (size_t i{0}; i < moons.size(); ++i) {
      for (size_t j{i + 1}; j < moons.size(); ++j) {
        moons[i].applyGrav(moons[j]);
      }
      moons[i].applyVel();
      //moons[i].print();
    }
    ++step;

    bool eqX{true}, eqY{true}, eqZ{true};

    for (size_t m{0}; m < moons.size(); ++m) {
      if (eqX && !doneX) eqX = eqX && (moons[m].pos.x == initial[m].pos.x) && (moons[m].vel.x == initial[m].vel.x);
      if (eqY && !doneY) eqY = eqY && (moons[m].pos.y == initial[m].pos.y) && (moons[m].vel.y == initial[m].vel.y);
      if (eqZ && !doneZ) eqZ = eqZ && (moons[m].pos.z == initial[m].pos.z) && (moons[m].vel.z == initial[m].vel.z);
    }

    if (eqX && !doneX) {
      stepX = step;
      doneX = true;
      std::cout << "Found X: " << stepX << "\n";
    }
    if (eqY && !doneY) {
      stepY = step;
      doneY = true;
      std::cout << "Found Y: " << stepY << "\n";
    }
    if (eqZ && !doneZ) {
      stepZ = step;
      doneZ = true;
      std::cout << "Found Z: " << stepZ << "\n";
    }
  }

  return lcm(stepX, lcm(stepY, stepZ));
}

void day12(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      slist.fromString(
          "<x=-1, y=0, z=2>\n"
          "<x=2, y=-10, z=-7>\n"
          "<x=4, y=-8, z=8>\n"
          "<x=3, y=5, z=-1>");
      runTest(day12_1(slist, 10), 179);

      slist.fromString(
          "<x=-8, y=-10, z=0>\n"
          "<x=5, y=5, z=10>\n"
          "<x=2, y=-7, z=3>\n"
          "<x=9, y=-8, z=-3>\n");
      runTest(day12_1(slist, 100), 1940);
    } else {
      slist.fromString(
          "<x=-1, y=0, z=2>\n"
          "<x=2, y=-10, z=-7>\n"
          "<x=4, y=-8, z=8>\n"
          "<x=3, y=5, z=-1>");
      runTest(day12_2(slist), 2772);

      slist.fromString(
          "<x=-8, y=-10, z=0>\n"
          "<x=5, y=5, z=10>\n"
          "<x=2, y=-7, z=3>\n"
          "<x=9, y=-8, z=-3>\n");
      runTest(day12_2(slist), 4686774924);
    }

  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day12_1(slist, 1000) << "\n";

      } else {
        std::cout << "result: " << day12_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}