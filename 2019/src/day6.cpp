#include <bits/stdc++.h>

#include "solutions.h"

struct OrbitObject {
  std::string parent;
  std::unordered_set<std::string> childs;
};

using OrbitObject_t = struct OrbitObject;

int day6_1(const Stringlist &input) {
  std::unordered_map<std::string, OrbitObject_t> objects;
  std::stack<std::pair<std::string, int>> objStack;
  int sum{0};

  for (auto &s : input) {
    size_t pos = s.find(')');
    std::string parentName{s.substr(0, pos)};
    std::string childName{s.substr(pos + 1)};

    objects[parentName].childs.insert(childName);
    objects[childName].parent = parentName;
  }

  objStack.push(std::make_pair("COM", 0));
  while (!objStack.empty()) {
    std::string objName{objStack.top().first};
    int depth{objStack.top().second};

    auto &obj{objects[objName]};

    sum += depth;
    objStack.pop();

    for (auto &childName : obj.childs) {
      objStack.push(std::make_pair(childName, depth + 1));
    }
  }

  return sum;
}
int day6_2(const Stringlist &input) {
  std::unordered_map<std::string, OrbitObject_t> objects;
  int result{0};

  for (auto &s : input) {
    size_t pos = s.find(')');
    std::string parentName{s.substr(0, pos)};
    std::string childName{s.substr(pos + 1)};

    objects[parentName].childs.insert(childName);
    objects[childName].parent = parentName;
  }

  std::unordered_map<std::string, int> santaPath;
  std::string curObj{"SAN"};
  int depth{0};
  while (curObj != "COM") {
    auto &obj = objects[curObj];
    curObj = obj.parent;
    santaPath[curObj] = depth++;
  }

  curObj = "YOU";
  depth = 0;
  while (curObj != "COM") {
    auto &obj = objects[curObj];
    curObj = obj.parent;
    if (santaPath.count(curObj) != 0) {
      result = depth + santaPath[curObj];
      break;
    }
    ++depth;
  }

  return result;
}

void day6(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      slist.fromString("COM)B\nB)C\nC)D\nD)E\nE)F\nB)G\nG)H\nD)I\nE)J\nJ)K\nK)L");
      runTest(day6_1(slist), 42);
    } else {
      slist.fromString("COM)B\nB)C\nC)D\nD)E\nE)F\nB)G\nG)H\nD)I\nE)J\nJ)K\nK)L\nK)YOU\nI)SAN");
      runTest(day6_2(slist), 4);
    }
  } else {
    if (slist.fromFile(filename)) {
      int result;

      result = (part == 1) ? day6_1(slist) : day6_2(slist);
      std::cout << "result: " << result << "\n";

    } else {
      std::cout << "file not found!\n";
    }
  }
}