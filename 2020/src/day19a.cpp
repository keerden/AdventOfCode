#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using matchSet =
    std::unordered_set<size_t>;  // for each found match contains position to
                                 // next portion of string to match

namespace {
class Rule {
 public:
  virtual matchSet match(std::map<int, Rule *> &ruleset, const std::string &msg,
                         matchSet &pos) = 0;
  virtual void printRule() = 0;
};

class LiteralRule : public Rule {
 private:
  char m_match;

 public:
  LiteralRule(char c) : m_match{c} {}

  matchSet match(std::map<int, Rule *> &ruleset, const std::string &msg,
                 matchSet &pos) {
    matchSet result;

    for (auto p : pos) {
      if (p < msg.length() && msg[p] == m_match) {
        ++p;
        result.insert(p);
      }
    }
    return result;
  }

  void printRule() { std::cout << "literal \"" << m_match << "\"\n"; }
};

class SubRule : public Rule {
 private:
  std::vector<int> m_subFirst;
  std::vector<int> m_subSecond;

 public:
  SubRule(std::vector<int> &first, std::vector<int> &second) {
    m_subFirst = std::move(first);
    m_subSecond = std::move(second);
  }
  SubRule(std::vector<int> &first) { m_subFirst = std::move(first); }

  matchSet match(std::map<int, Rule *> &ruleset, const std::string &msg,
                 matchSet &posset) {
    matchSet result{};
    for (auto pos : posset) {
      matchSet matches{true};
      matchSet leftMatch{pos};
      // match the first list
      for (auto r : m_subFirst) {
        auto it = ruleset.find(r);
        assert(it != ruleset.end());

        leftMatch = std::move(it->second->match(ruleset, msg, leftMatch));
        if (leftMatch.empty()) break;
      }

      result.insert(leftMatch.begin(), leftMatch.end());

      if(!m_subSecond.empty()){
        matchSet rightMatch{pos};
        for (auto r : m_subSecond) {
          auto it = ruleset.find(r);
          assert(it != ruleset.end());
          rightMatch = std::move(it->second->match(ruleset, msg, rightMatch));
          if (rightMatch.empty()) break;
        }
        result.insert(rightMatch.begin(), rightMatch.end());
      }  
    }

        return result;
  }

  void printRule() {
    std::cout << "subrules";
    for (auto r : m_subFirst) std::cout << " " << r;

    if (m_subSecond.size() > 0) {
      std::cout << " |";
      for (auto r : m_subSecond) std::cout << " " << r;
    }
    std::cout << "\n";
  }
};

class RuleSet {
 private:
  std::map<int, Rule *> m_rules;

 public:
  RuleSet(std::vector<std::string> &rules) {
    for (auto &s : rules) {
      size_t pos{0};
      size_t ruleNum;
      ruleNum = std::stoi(s, &pos);
      pos += 2;  // ": ";
      if (s[pos] == '\"') {
        m_rules[ruleNum] = new LiteralRule(s[pos + 1]);
      } else {
        std::stringstream ss(s.substr(pos));
        std::string token;
        std::vector<int> subFirst;

        while (ss >> token && token != "|") {
          size_t subrule = std::stoi(token);
          subFirst.push_back(subrule);
        }

        if (token == "|") {
          std::vector<int> subSecond;
          while (ss >> token) {
            size_t subrule = std::stoi(token);
            subSecond.push_back(subrule);
          }
          m_rules[ruleNum] = new SubRule(subFirst, subSecond);
        } else {
          m_rules[ruleNum] = new SubRule(subFirst);
        }
      }
    }
  }

  int match(std::vector<std::string> &messages) {
    int result{0};
    for (auto &m : messages) {
      matchSet input{0};
      matchSet output = m_rules[0]->match(m_rules, m, input);
     // std::cout << "matching: " << m << "\n";
      for(auto pos: output){
        if (pos == m.length()) {
          result++;
          std::cout << "matched: " << m << "\n";
          break;
        }
      }
    }
    return result;
  }

  void printRules() {
    std::cout << "ruleset:\n";
    for (auto r : m_rules) {
      std::cout << "\t" << r.first << ": ";
      r.second->printRule();
    }
  }
};
}  // namespace

int main() {
  std::string line;
  std::ifstream file;
  std::vector<std::string> rules;
  std::vector<std::string> messages;

  long long result{0};

  file.open("input/day19.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  while (getline(file, line)) {  // collect rules
    if (line == "") break;
    if (line == "8: 42") line = "8: 42 | 42 8";
    if (line == "11: 42 31") line = "11: 42 31 | 42 11 31";
    rules.push_back(std::move(line));
  }

  while (getline(file, line)) {  // collect messages
    messages.push_back(std::move(line));
  }

  RuleSet r(rules);
  //r.printRules();
  result = r.match(messages);

  std::cout << "result: " << result << "\n";

  return 0;
}