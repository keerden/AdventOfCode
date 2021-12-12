#include <bits/stdc++.h>

#include "solutions.h"

namespace {

class caveMap {
 private:
  std::unordered_multimap<std::string, std::string> cmap;
  std::unordered_map<std::string, bool> mapSmall;

 public:
  caveMap(){};
  caveMap(const Stringlist &inp) {
    std::regex reg{"^(\\S+)-(\\S+)$"};
    std::smatch sm;
    for (auto &l : inp) {
      if (!std::regex_match(l, sm, reg)) {
        std::cout << "MATCH ERROR [" << l << "]\n";
        continue;
      }
      cmap.insert(std::pair<std::string, std::string>(sm[1], sm[2]));
      cmap.insert(std::pair<std::string, std::string>(sm[2], sm[1]));
      mapSmall[sm[1]] = isSmall(sm[1]);
      mapSmall[sm[2]] = isSmall(sm[2]);
    }
  }

  int countPaths() {
    std::unordered_multiset<std::string> small_visited;

    return visit("start", small_visited, true);
  }

  int countPathsDouble() {
    std::unordered_multiset<std::string> small_visited;

    return visit("start", small_visited, false);
  }

 private:
  bool isSmall(std::string cave) {
    for (auto &c : cave) {
      if (isupper(c))
        return false;
    }
    return true;
  }

  int visit(std::string start, std::unordered_multiset<std::string> &small_visited, bool once) {
    int count{0};
    if (start == "end") {
      return 1;
    }

    if (mapSmall[start]) {
      small_visited.insert(start);
    }
    //check each edge if visited, otherwise visit
    auto range = cmap.equal_range(start);

    for (auto itt = range.first; itt != range.second; ++itt) {

      if (itt->second == "start")
        continue;

      if (once) {
        if (mapSmall[itt->second] && small_visited.contains(itt->second))
          continue;

        count += visit(itt->second, small_visited, true);
      } else {
        if (mapSmall[itt->second] && small_visited.count(itt->second) == 1) {
          count += visit(itt->second, small_visited, true);
        } else {
          count += visit(itt->second, small_visited, false);
        }
      }
    }
    auto itr = small_visited.find(start);
    if (itr != small_visited.end()) {
      small_visited.erase(itr);
    }

    return count;
  }
};

}  //namespace

int day12_1(const Stringlist &input) {
  caveMap cmap(input);

  return cmap.countPaths();
}
int day12_2(const Stringlist &input) {
  caveMap cmap(input);

  return cmap.countPathsDouble();
}

void day12(int part, bool test, std::string filename) {
  Stringlist slist, slist2, slist3;
  if (test) {
    slist.fromString(
        "start-A\n"
        "start-b\n"
        "A-c\n"
        "A-b\n"
        "b-d\n"
        "A-end\n"
        "b-end\n");
    slist2.fromString(
        "dc-end\n"
        "HN-start\n"
        "start-kj\n"
        "dc-start\n"
        "dc-HN\n"
        "LN-dc\n"
        "HN-end\n"
        "kj-sa\n"
        "kj-HN\n"
        "kj-dc\n");
    slist3.fromString(
        "fs-end\n"
        "he-DX\n"
        "fs-he\n"
        "start-DX\n"
        "pj-DX\n"
        "end-zg\n"
        "zg-sl\n"
        "zg-pj\n"
        "pj-he\n"
        "RW-he\n"
        "fs-DX\n"
        "pj-RW\n"
        "zg-RW\n"
        "start-pj\n"
        "he-WI\n"
        "zg-he\n"
        "pj-fs\n"
        "start-RW\n");

    if (part == 1) {
      runTest(day12_1(slist), 10);
      runTest(day12_1(slist2), 19);
      runTest(day12_1(slist3), 226);
    } else {
      runTest(day12_2(slist), 36);
      runTest(day12_2(slist2), 103);
      runTest(day12_2(slist3), 3509);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day12_1(slist) << "\n";
      } else {
        std::cout << "result: " << day12_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}