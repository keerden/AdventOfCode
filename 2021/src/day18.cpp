#include <bits/stdc++.h>

#include "solutions.h"

namespace {

class SnailfishNumber {
 private:
  bool isRegular;
  int number;
  SnailfishNumber *left;
  SnailfishNumber *right;

 public:
  SnailfishNumber() : SnailfishNumber(0){};
  SnailfishNumber(int num) : isRegular{true}, number{num}, left{NULL}, right{NULL} {}
  SnailfishNumber(SnailfishNumber *left, SnailfishNumber *right) : isRegular{false}, number{0}, left{left}, right{right} {}
  SnailfishNumber(const std::string &line) : left{NULL}, right{NULL} {
    std::string_view sv{line};
    parse(sv);
  }
  SnailfishNumber(std::string_view &sv) : left{NULL}, right{NULL} {
    parse(sv);
  }

  SnailfishNumber(const SnailfishNumber &other) {
    isRegular = other.isRegular;
    number = other.number;
    if (isRegular) {
      left = NULL;
      right = NULL;
    } else {
      left = new SnailfishNumber(*other.left);
      right = new SnailfishNumber(*other.right);
    }
  }

  ~SnailfishNumber() {
    delete left;
    delete right;
  }

  void print(bool newLine = false) {
    if (isRegular) {
      std::cout << this->number;
    } else {
      std::cout << "[";
      this->left->print();
      std::cout << ",";
      this->right->print();
      std::cout << "]";
    }
    if(newLine){
      std::cout << "\n";
    }
  }

  long long int magnitude() {
    if (isRegular)
      return number;

    return (3 * left->magnitude()) + (2 * right->magnitude());
  }

  SnailfishNumber &operator+=(const SnailfishNumber &rhs) {
    this->add(rhs);
    return *this;
  }

  SnailfishNumber operator+(const SnailfishNumber &rhs) const {
    SnailfishNumber ret{*this};
    ret.add(rhs);
    return ret;
  }

  SnailfishNumber &operator=(const SnailfishNumber &other){
    isRegular = other.isRegular;
    number = other.number;
    if(isRegular){
      left = NULL;
      right = NULL;
    }else{
      left = new SnailfishNumber(*left);
      right = new SnailfishNumber(*right);
    }
    return *this;
  }

  bool parse(const std::string &line){
    std::string_view sv{line};
    return parse(sv);
  }

  bool parse(std::string_view &line) {
    SnailfishNumber *tokenLeft, *tokenRight;
    delete left;
    delete right;
    isRegular = true;
    number = 0;

    if (line[0] != '[')
      return false;

    line.remove_prefix(1);
    tokenLeft = parseHalf(line);

    if (!tokenLeft)
      return false;

    if (line[0] != ',')
      return false;

    line.remove_prefix(1);

    tokenRight = parseHalf(line);

    if (!tokenRight) {
      delete tokenLeft;
      return false;
    }

    if (line[0] != ']') {
      delete tokenLeft;
      delete tokenRight;
      return false;
    }
    line.remove_prefix(1);

    isRegular = false;
    left = tokenLeft;
    right = tokenRight;
    number = 0;

    return true;
  }

 private:
  void add(const SnailfishNumber &other) {
    if (isRegular) {
      isRegular = false;
      left = new SnailfishNumber(number);
    } else {
      SnailfishNumber *newLeft = new SnailfishNumber(left, right);
      left = newLeft;
    }
    right = new SnailfishNumber(other);
    reduce();
  }

  SnailfishNumber *parseHalf(std::string_view &line) {
    SnailfishNumber *result;
    if (line[0] == '[') {
      result = new SnailfishNumber(line);
    } else if (line[0] >= '0' && line[0] <= '9') {
      result = new SnailfishNumber(line[0] - '0');
      line.remove_prefix(1);
    } else {  //parse error
      return NULL;
    }

    return result;
  }

  void reduce() {

    do {
      explode();
    } while (split());
  }

  bool explode() {
    std::stack<SnailfishNumber *> parents;
    std::stack<SnailfishNumber *> regulars;
    SnailfishNumber *cur = this;
    bool exploded{false}, backtrack{false};
    int addRight;

    do {
      if (backtrack) {
        if (parents.top()->left == cur) {
          cur = parents.top()->right;
          backtrack = false;
        } else {
          cur = parents.top();
          parents.pop();
          continue;
        }
      }

      if (cur->isRegular) {
        if (exploded) {
          cur->number += addRight;
          exploded = false;
        }
        regulars.push(cur);
        backtrack = true;
        
      } else {
        if (parents.size() == 4) {  //explode        
          assert(cur->left->isRegular && cur->right->isRegular);
          if(exploded){  //if exploded addright first
            cur->left->number += addRight;
          }

          if (!regulars.empty()) {
            regulars.top()->number += cur->left->number;
          }
          addRight = cur->right->number;

          delete cur->left;
          cur->left = NULL;
          delete cur->right;
          cur->right = NULL;
          cur->number = 0;
          cur->isRegular = true;
          regulars.push(cur);
          exploded = true;
          backtrack = true;
        } else {
          parents.push(cur);
          cur = cur->left;
        }
      }
    } while (!parents.empty());

    return exploded;
  }

  bool split() {
    std::stack<SnailfishNumber *> parents;
    SnailfishNumber *cur = this;
    bool backtrack{false};

    do {
      if (backtrack) {
        if (parents.top()->left == cur) {
          cur = parents.top()->right;
          backtrack = false;
        } else {
          cur = parents.top();
          parents.pop();
          continue;
        }
      }

      if (cur->isRegular) {
        //std::cout << "S->d: " << parents.size() << " num = " << cur->number << "\n";
        if (cur->number >= 10) {
          cur->left = new SnailfishNumber(cur->number / 2);
          cur->right = new SnailfishNumber(cur->number / 2);
          if (cur->number % 2)
            cur->right->number++;
          cur->isRegular = false;
          return true;
        }
        backtrack = true;
      } else {
        //std::cout << "S->d: " << parents.size() << " cur ->\t";
        //cur->print();
        //std::cout << "\n";

        parents.push(cur);
        cur = cur->left;
      }
    } while (!parents.empty());

    return false;
  }
};

}  //namespace

long long int day18_1(const Stringlist &input) {
  SnailfishNumber snum;
  bool first = true;

  for (auto &line : input) {
    if (first) {
      first = false;
      snum.parse(line);
    } else {
      snum += SnailfishNumber(line);
    }
  }
  return snum.magnitude();
}
long long int day18_2(const Stringlist &input) {
  long long int max = LLONG_MIN;
  for (auto itt1 = input.begin(); itt1 != input.end(); ++itt1) {
    SnailfishNumber snum1{*itt1};
    for (auto itt2 = itt1 + 1; itt2 != input.end(); ++itt2) {
      SnailfishNumber snum2{*itt2};
      max = std::max(max, (snum1 + snum2).magnitude());
      max = std::max(max, (snum2 + snum1).magnitude());
    }
  }

  return max;
}

void day18(int part, bool test, std::string filename) {
  Stringlist slist, slist2;
  if (test) {
    slist.fromString(
        "[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]\n"
        "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]\n"
        "[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]\n"
        "[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]\n"
        "[7,[5,[[3,8],[1,4]]]]\n"
        "[[2,[2,2]],[8,[8,1]]]\n"
        "[2,9]\n"
        "[1,[[[9,3],9],[[9,0],[0,7]]]]\n"
        "[[[5,[7,4]],7],1]\n"
        "[[[[4,2],2],6],[8,7]]");
    slist2.fromString(
        "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]\n"
        "[[[5,[2,8]],4],[5,[[9,9],0]]]\n"
        "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]\n"
        "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]\n"
        "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]\n"
        "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]\n"
        "[[[[5,4],[7,7]],8],[[8,3],8]]\n"
        "[[9,3],[[9,9],[6,[4,9]]]]\n"
        "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]\n"
        "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]");
    if (part == 1) {
      runTest(day18_1(slist), 3488);
      runTest(day18_1(slist2), 4140);
    } else {
      runTest(day18_2(slist2), 3993);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day18_1(slist) << "\n";
      } else {
        std::cout << "result: " << day18_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}