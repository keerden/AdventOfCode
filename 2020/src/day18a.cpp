#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::vector;

enum EXP { NONE, MUL, ADD };

long long parseSection(std::string str, size_t &pos) {
  EXP activeExp = NONE;
  long long mulbuf{1};
  long long result{0};
  long long num{};

  while (pos < str.length() && str[pos] != ')') {
    while (str[pos] == ' ') pos++;

    if (isdigit(str[pos])) {  // parse num
      int start = pos;
      while (isdigit(str[pos])) pos++;
      num = stoi(str.substr(start, pos - start));
      switch (activeExp) {
        case NONE:
          result = num;
          break;
        case ADD:
          result += num;
          break;
        default:
          break;
      }
    } else {
      switch (str[pos]) {
        case '+':
          activeExp = ADD;
          ++pos;
          break;
        case '*':
          activeExp = NONE;
          mulbuf *= result;
          result = 0;
          ++pos;
          break;
        case '(':
          ++pos;
          num = parseSection(str, pos);
          switch (activeExp) {
            case NONE:
              result = num;
              break;
            case ADD:
              result += num;
              break;
            case MUL:
              result *= num;
              break;
          }
          break;
        default:
          ++pos;
          break;
      }
    }
  }
  ++pos;
 // std::cout << "res: " << result << "\n";
  return result * mulbuf;
}

int main() {
  std::string line;
  std::ifstream file;

  long long result{0};

  file.open("input/day18.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  while (getline(file, line)) {
    size_t pos = 0;
    long long num = parseSection(line, pos);
    std::cout << line << " = " << num << "\n";
    result += num;
  }


  std::cout << "result: " << result << "\n";

  return 0;
}