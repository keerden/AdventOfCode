#include <bits/stdc++.h>

#include "solutions.h"

namespace {
class Paper {
 private:
  std::set<Position2D> points;
  std::list<Position2D> folds;
  int xmin, xmax;
  int ymin, ymax;

 public:
  Paper() : xmin{0}, xmax{0}, ymin{0}, ymax{0} {}
  Paper(const Stringlist &input) {
    bool folding = false;
    xmin = 0;
    xmax = 0;
    ymin = 0;
    ymax = 0;

    for (auto &l : input) {
      std::string token;
      if (l == "") {
        folding = true;
        continue;
      }

      if (folding) {
        token = l.substr(13);
        if (l[11] == 'x') {
          folds.emplace_back(Position2D(stoi(token), -1));
        } else {
          folds.emplace_back(Position2D(-1, stoi(token)));
        }

      } else {
        size_t sep = l.find(",");
        int x = stoi(l.substr(0, sep));
        int y = stoi(l.substr(sep + 1));

        xmax = std::max(xmax, x);
        ymax = std::max(ymax, y);

        points.emplace(Position2D(x, y));
      }
    }
  }
  void printPoints() {
    std::cout << "Points:\n";
    for (auto &p : points) {
      std::cout << p << "\n";
    }
  }

  void print(){
    for(int y{0}; y <= ymax; ++y){
      for(int x{0}; x <= xmax; ++x){
        if(points.contains(Position2D(x,y))){
          std::cout << "#";
        }else{
          std::cout << " ";
        }
      }
      std::cout << "\n";
    }
  }

  void fold() {
    size_t num = folds.size();
    for (size_t i{0}; i < num; ++i) {
      foldOnce();
    }
  }

  void foldOnce() {
    if (folds.empty())
      return;

    Position2D fold = folds.front();
    folds.pop_front();

    xmax = (fold.x == -1) ? xmax : (fold.x - 1);
    ymax = (fold.y == -1) ? ymax : (fold.y - 1);

    auto itt = points.begin();

    while (itt != points.end()) {
      if (fold.x == -1) {  //fold y

        if (itt->y > fold.y) {
          Position2D pos = *itt;
          auto oldItt = itt;
          ++itt;
          points.erase(oldItt);
          pos.y = fold.y - (pos.y - fold.y);
          points.insert(pos);
          continue;
        }
      } else {  //fold x
        if (itt->x > fold.x) {
          Position2D pos = *itt;
          auto oldItt = itt;
          ++itt;
          points.erase(oldItt);
          pos.x = fold.x - (pos.x - fold.x);
          points.insert(pos);
          continue;
        }
      }
      ++itt;
    }
  }

  size_t countPoints() {
    return points.size();
  }
};

}  //namespace

int day13_1(const Stringlist &input) {
  Paper p(input);
  p.foldOnce();
  p.print();
  return p.countPoints();
}
int day13_2(const Stringlist &input) {
  Paper p(input);
  p.fold();
  std::cout << "\n";
  p.print();
  return 0;
}

void day13(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "6,10\n"
        "0,14\n"
        "9,10\n"
        "0,3\n"
        "10,4\n"
        "4,11\n"
        "6,0\n"
        "6,12\n"
        "4,1\n"
        "0,13\n"
        "10,12\n"
        "3,4\n"
        "3,0\n"
        "8,4\n"
        "1,10\n"
        "2,14\n"
        "8,10\n"
        "9,0\n"
        "\n"
        "fold along y=7\n"
        "fold along x=5\n");
    if (part == 1) {
      runTest(day13_1(slist), 17);
    } else {
      runTest(day13_2(slist), 0);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day13_1(slist) << "\n";
      } else {
        std::cout << "result: " << day13_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}