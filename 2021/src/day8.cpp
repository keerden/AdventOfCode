#include <bits/stdc++.h>

#include "solutions.h"

namespace {
class Digit {
 private:
  std::unordered_set<char> segments;

 public:
  Digit() {}
  Digit(std::string str) {
    for (auto &c : str) {
      segments.insert(c);
    }
  }

  int size(void) {
    return segments.size();
  }

  bool contains(const Digit &other) const {
    if (segments.size() < other.segments.size())
      return false;

    for (auto &c : other.segments) {
      if (this->segments.count(c) == 0)
        return false;
    }
    return true;
  }

  bool operator==(const Digit &other) const {
    if (segments.size() != other.segments.size())
      return false;

    for (auto &c : segments) {
      if (other.segments.count(c) == 0)
        return false;
    }
    return true;
  }

  friend std::ostream &operator<<(std::ostream &output, const Digit &d) {
    for (auto &c : d.segments) {
      output << c;
    }
    return output;
  }
};

}  //namespace

int day8_1(const Stringlist &input) {
  std::regex reg{"^(\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) \\| (\\S+) (\\S+) (\\S+) (\\S+)$"};
  std::smatch sm;
  std::string comb[10];
  std::string output[4];
  int count{0};
  for (auto &line : input) {
    if (!std::regex_match(line, sm, reg)) {
      std::cout << "MATCH ERROR [" << line << "]\n";
      return 0;
    }
    for (int i{0}; i < 10; ++i) {
      comb[i] = sm[i + 1];
      if (i < 4) {
        output[i] = sm[i + 11];
        switch (output[i].length()) {
          case 2:  //1
          case 3:  //7
          case 4:  //4
          case 7:  //8
            ++count;
            break;
          default:
            break;
        }
      }
    }
  }

  return count;
}
int resolve(std::string line) {
  Digit digit;
  Digit knownDigits[10];
  Digit output[4];
  std::list<Digit> digits5;
  std::list<Digit> digits6;

  std::regex reg{"^(\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) (\\S+) \\| (\\S+) (\\S+) (\\S+) (\\S+)$"};
  std::smatch sm;

  if (!std::regex_match(line, sm, reg)) {
    std::cout << "MATCH ERROR [" << line << "]\n";
    return -1;
  }
  for (int i{0}; i < 4; ++i) {
    output[i] = std::move(Digit(sm[i + 11]));
  }
  for (int i{0}; i < 10; ++i) {
    digit = Digit(sm[i + 1]);

    switch (digit.size()) {
      case 2:
        knownDigits[1] = std::move(digit);
        break;
      case 3:
        knownDigits[7] = std::move(digit);
        break;
      case 4:
        knownDigits[4] = std::move(digit);
        break;
      case 5:
        digits5.push_back(std::move(digit));
        break;
      case 6:
        digits6.push_back(std::move(digit));
        break;
      case 7:
        knownDigits[8] = std::move(digit);
        break;
      default:
        return -1;
    }
  }

  //out of 3 5 2, find 3 using 7

  for (auto itt = digits5.begin(); itt != digits5.end(); ++itt) {
    if (itt->contains(knownDigits[7])) {
      knownDigits[3] = std::move(*itt);
      digits5.erase(itt);
      break;
    }
  }
  assert(digits5.size() == 2);
  //out of 0 6 9 find nr 6 with 7
  for (auto itt = digits6.begin(); itt != digits6.end(); ++itt) {
    if (!itt->contains(knownDigits[7])) {
      knownDigits[6] = std::move(*itt);
      digits6.erase(itt);
      break;
    }
  }
  assert(digits6.size() == 2);

  //out of 0 9 find nr 9 with 3
  for (auto itt = digits6.begin(); itt != digits6.end(); ++itt) {
    if (itt->contains(knownDigits[3])) {
      knownDigits[9] = std::move(*itt);
      digits6.erase(itt);
      break;
    }
  }
  assert(digits6.size() == 1);
  knownDigits[0] = std::move(digits6.front());

  // out of 5 2, find 5 with 6, and remainder 2
  if (knownDigits[6].contains(digits5.front())) {
    knownDigits[5] = std::move(digits5.front());
    knownDigits[2] = std::move(digits5.back());
  } else {
    knownDigits[5] = std::move(digits5.back());
    knownDigits[2] = std::move(digits5.front());
  }

  int result{0};

  for (int i{0}; i < 4; ++i) {
    result *= 10;
    for (int j{0}; j < 10; ++j) {
      if (output[i] == knownDigits[j]) {
        result += j;
        break;
      }
    }
  }
  return result;
}

int day8_2(const Stringlist &input) {
  int count{0};

  for (auto &l : input) {
    int num = resolve(l);
    if (num == -1) {
      std::cout << "Error resolving: " << l << "\n";
      return -1;
    }
    count += num;
  }

  return count;
}

void day8(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe\n"
        "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc\n"
        "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg\n"
        "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb\n"
        "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea\n"
        "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb\n"
        "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe\n"
        "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef\n"
        "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb\n"
        "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce\n");
    if (part == 1) {
      runTest(day8_1(slist), 26);
    } else {
      runTest(day8_2(slist), 61229);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day8_1(slist) << "\n";
      } else {
        std::cout << "result: " << day8_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}