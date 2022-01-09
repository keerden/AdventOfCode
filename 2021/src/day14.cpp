#include <bits/stdc++.h>

#include "solutions.h"

namespace {
class CharPair {
 public:
  char a;
  char b;
  CharPair(char a, char b) : a{a}, b{b} {};
  CharPair(void) : a{0}, b{0} {};

  bool operator==(const CharPair &rhs) const {
    return (a == rhs.a) && (b == rhs.b);
  }
  friend std::ostream &operator<<(std::ostream &output, const CharPair &p) {
    output << p.a << p.b;
    return output;
  }
};
}  //namespace

namespace std {

template <>
struct hash<CharPair> {
  std::size_t operator()(const CharPair &p) const {
    using std::hash;

    return (hash<int>()(p.a) ^ (hash<int>()(p.b) << 1));
  }
};
}  //namespace std

namespace {

struct ArrayHasher {
  std::size_t operator()(const std::array<char, 2> &a) const {
    std::size_t h = 0;

    for (auto e : a) {
      h ^= std::hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
  }
};

class Polymer {
 private:
 private:
  std::unordered_map<std::array<char, 2>, char, ArrayHasher> rules;
  std::unordered_map<std::array<char, 2>, long long unsigned int, ArrayHasher> resultMap;
  std::string templ;

 public:
  Polymer() {}
  explicit Polymer(const Stringlist &input) {
    //first line = template
    templ = input.get(0);
    for (size_t i{1}; i < templ.length(); ++i) {
      ++resultMap[{templ[i - 1], templ[i]}];
    }

    auto itt = input.begin() + 2;

    while (itt != input.end()) {
      rules[{(*itt)[0], (*itt)[1]}] = (*itt)[6];
      ++itt;
    }
  }

  void step(size_t n) {
    for (size_t i{0}; i < n; ++i){
      step();
    }
  }

  void step() {
    std::unordered_map<std::array<char, 2>, long long unsigned int, ArrayHasher> newResult;

    for (auto [pair, count] : resultMap) {
      char c = rules[pair];

      newResult[{pair[0], c}] += count;
      newResult[{c, pair[1]}] += count;

      //std::cout << pair[0] << pair[1] << " -----> " <<c << " --- " << count << "\n";
      //for (auto [pair, count] : newResult) {
      //  std::cout << pair[0] << pair[1] << " = " << count << "\n";
      //}
      //std::cout << "\n";
    }
    resultMap = std::move(newResult);
  }

  void print() {
    for (auto [pair, count] : resultMap) {
      std::cout << pair[0] << pair[1] << " = " << count << "\n";
    }
    std::cout << "\n";
  }

  unsigned long long result() {
    unsigned long long max{0}, min{ULLONG_MAX};
    std::unordered_map<char, unsigned long long> charCounts;
    charCounts[templ[0]] = 1;
    for (auto [pair, count] : resultMap) {
      charCounts[pair[1]] += count;
    }
    for (auto s : charCounts) {
      max = std::max(max, s.second);
      min = std::min(min, s.second);
    }

    return max - min;
  }
};
}  //namespace

unsigned long long day14_1(const Stringlist &input) {
  Polymer p(input);
  p.step(10);
  return p.result();
}
unsigned long long day14_2(const Stringlist &input) {
  Polymer p(input);
  p.step(40);
  return p.result();
}

void day14(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "NNCB\n"
        "\n"
        "CH -> B\n"
        "HH -> N\n"
        "CB -> H\n"
        "NH -> C\n"
        "HB -> C\n"
        "HC -> B\n"
        "HN -> C\n"
        "NN -> C\n"
        "BH -> H\n"
        "NC -> B\n"
        "NB -> B\n"
        "BN -> B\n"
        "BB -> N\n"
        "BC -> B\n"
        "CC -> N\n"
        "CN -> C\n");
    if (part == 1) {
      runTest(day14_1(slist), 1588u);
    } else {
      runTest(day14_2(slist), 2188189693529u);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day14_1(slist) << "\n";
      } else {
        std::cout << "result: " << day14_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}