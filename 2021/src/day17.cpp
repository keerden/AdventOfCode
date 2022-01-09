#include <bits/stdc++.h>

#include "solutions.h"

struct Region {
  int x1, x2;
  int y1, y2;
  Region(int x1, int x2, int y1, int y2) : x1{x1}, x2{x2}, y1{y1}, y2{y2} {}
};

struct SolutionsY {
  int minStep = -1;
  int maxStep = -1;
  int heighest = INT_MIN;
};

SolutionsY calcStepsVelY(int velY, Region r) {
  SolutionsY result;
  int step{0};
  for (int y{0}; y >= r.y1; ++step, --velY, y += velY) {
    if (y <= r.y2) {
      if (result.minStep == -1)
        result.minStep = step;
      result.maxStep = step;
    }
    result.heighest = std::max(result.heighest, y);
  }

  return result;
}

int calcX(int step, int velX) {
  if (step >= velX) {
    return (velX + 1) * velX / 2;
  }

  return (velX + (velX - step + 1)) * step / 2;
}

int day17_1(Region r) {
  int minVelX = 0;
  int maxVelX = r.x2;
  int minVelY = r.y1;
  int maxVelY = -r.y1;

  for (int velY{maxVelY}; velY >= minVelY; --velY) {
    auto solY = calcStepsVelY(velY, r);
    if (solY.maxStep == -1 || solY.minStep == -1)
      continue;

    for (int step{solY.minStep}; step <= solY.maxStep; ++step) {
      for (int velX{minVelX}; velX <= maxVelX; ++velX) {
        int x = calcX(step, velX);
        if (x >= r.x1 && x <= r.x2) {
          return solY.heighest;
        }
      }
    }
  }

  return 0;
}
int day17_2(Region r) {
  int minVelX = 0;
  int maxVelX = r.x2;
  int minVelY = r.y1;
  int maxVelY = -r.y1;
  int valid{0};

  for (int velY{maxVelY}; velY >= minVelY; --velY) {
    auto solY = calcStepsVelY(velY, r);
    if (solY.maxStep == -1 || solY.minStep == -1)
      continue;

    for (int velX{minVelX}; velX <= maxVelX; ++velX) {
      for (int step{solY.minStep}; step <= solY.maxStep; ++step) {
        int x = calcX(step, velX);
        if (x >= r.x1 && x <= r.x2) {
          ++valid;
          break;
        }
      }
    }
  }

  return valid;
}

void day17(int part, bool test, std::string filename) {
  if (test) {
    if (part == 1) {
      runTest(day17_1(Region(20, 30, -10, -5)), 45);
    } else {
      runTest(day17_2(Region(20, 30, -10, -5)), 112);
    }
  } else {
    if (part == 1) {
      std::cout << "result: " << day17_1(Region(248, 285, -85, -56)) << "\n";
    } else {
      std::cout << "result: " << day17_2(Region(248, 285, -85, -56)) << "\n";
    }
  }
}