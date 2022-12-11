#include <bits/stdc++.h>

#include "solutions.h"

const int ROCK = 1;
const int PAPER = 2;
const int SCISSORS = 3;
const int WIN = 6;
const int DRAW = 3;
const int LOST = 0;

int play(int opponent, int own) {
    int result{0};
    switch (own) {
        case ROCK:
            result = (opponent == SCISSORS) ? WIN : ((opponent == PAPER) ? LOST : DRAW); 
            break;
        case PAPER:
            result = (opponent == ROCK) ? WIN : ((opponent == SCISSORS) ? LOST : DRAW); 
            break;
        case SCISSORS:
            result = (opponent == PAPER) ? WIN : ((opponent == ROCK) ? LOST : DRAW); 
            break;
    }
    result += own;
    return result;
}

int playOutcome(int opponent, int outcome) {
    int result{0};
    switch (outcome) {
        case 'X': //lose
            result = LOST + ((opponent == SCISSORS) ? PAPER : ((opponent == PAPER) ? ROCK : SCISSORS)); 
            break;
        case 'Y': //draw
            result = DRAW + opponent; 
            break;
        case 'Z': //win
            result = WIN + ((opponent == SCISSORS) ? ROCK : ((opponent == PAPER) ? SCISSORS : PAPER));  
            break;
    }
    return result;
}

int day2_1(const Stringlist &input) {
  int score{0};

  for (auto &line : input) {
    std::stringstream ss{line};
    std::string own;
    std::string opponent;
    getline(ss, own, ' ');
    getline(ss, opponent, ' ');
    score += play(own[0] - 'A' + 1, opponent[0] - 'X' + 1);
  }

  return score;
}
int day2_2(const Stringlist &input) {
  int score{0};

  for (auto &line : input) {
    std::stringstream ss{line};
    std::string own;
    std::string opponent;
    getline(ss, own, ' ');
    getline(ss, opponent, ' ');
    score += playOutcome(own[0] - 'A' + 1, opponent[0]);
  }

  return score;
}

void day2(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "A Y\n"
        "B X\n"
        "C Z\n");
    if (part == 1) {
      runTest(day2_1(slist), 15);
    } else {
      runTest(day2_2(slist), 12);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day2_1(slist) << "\n";
      } else {
        std::cout << "result: " << day2_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}