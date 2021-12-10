#include <bits/stdc++.h>

#include "solutions.h"

bool isOpenBracket(char c) {
  return (c == '(' || c == '[' || c == '{' || c == '<');
}

bool isMatchingBracket(char open, char close) {
  switch (open) {
    case '(':
      return close == ')';
    case '[':
      return close == ']';
    case '{':
      return close == '}';
    case '<':
      return close == '>';
  }
  return false;
}

long long int checkSyntack(const std::string &str) {
  std::stack<char> openBrackets;

  for (auto &c : str) {
    if (isOpenBracket(c)) {
      openBrackets.push(c);
    } else {  //closing
      if (openBrackets.empty())
        return 0;  //missing open, ignore line for now

      if (isMatchingBracket(openBrackets.top(), c)) {
        openBrackets.pop();
      } else {  //invalid closing bracket
        switch (c) {
          case ')':
            return 3;
          case ']':
            return 57;
          case '}':
            return 1197;
          case '>':
            return 25137;
        }
      }
    }
  }

  if (!openBrackets.empty()) {
    long long int score = 0;
    while (!openBrackets.empty()) {
      char c = openBrackets.top();
      openBrackets.pop();
      int points = 0;
      switch(c) {
        case '(':
          points = 1;
          break;
        case '[':
          points = 2;
          break;
        case '{':
          points = 3;
          break;
        case '<':
          points = 4;
          break;
      }
      score = (score * 5) + points;
    }
    return -score;
  }

  return 0;  //ok
}

long long int day10_1(const Stringlist &input) {
  long long int sum{0};
  long long int score;

  for (auto &line : input) {
    score = checkSyntack(line);
    if (score > 0)
      sum += score;
  }
  return sum;
}
long long int day10_2(const Stringlist &input) {
  std::vector<long long int> scores;
  long long int score;

  for (auto &line : input) {
    score = checkSyntack(line);
    if (score < 0)
      scores.push_back(-score);
  }

  size_t middle = scores.size() / 2;
  std::sort (scores.begin(), scores.end());

  return scores[middle];
}

void day10(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "[({(<(())[]>[[{[]{<()<>>\n"
        "[(()[<>])]({[<{<<[]>>(\n"
        "{([(<{}[<>[]}>{[]{[(<()>\n"
        "(((({<>}<{<{<>}{[]{[]{}\n"
        "[[<[([]))<([[{}[[()]]]\n"
        "[{[{({}]{}}([{[{{{}}([]\n"
        "{<[[]]>}<{[{[{[]{()[[[]\n"
        "[<(<(<(<{}))><([]([]()\n"
        "<{([([[(<>()){}]>(<<{{\n"
        "<{([{{}}[<[[[<>{}]]]>[]]\n");
    if (part == 1) {
      runTest(day10_1(slist), 26397);
    } else {
      runTest(day10_2(slist), 288957);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day10_1(slist) << "\n";
      } else {
        std::cout << "result: " << day10_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}