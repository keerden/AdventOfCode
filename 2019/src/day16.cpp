#include <bits/stdc++.h>

#include "intcode.h"
#include "util.h"

void genSequence(std::vector<int> &seqBuf, size_t len, int iter) {
  std::vector<int> result;
  int sequence[4] = {0, 1, 0, -1};

  for (size_t i{0}; i < len; ++i) {
    seqBuf[i] = sequence[((i + 1) / (iter + 1)) & 3];
  }
}

std::string day16_1(const std::string &line, int times = 100) {
  std::string result;

  size_t len = line.size();
  std::vector<int> seqBuf(len);
  std::vector<int> input(len);
  std::vector<int> calculated;

  for (auto &c : line) {
    calculated.push_back(c - '0');
  }

  for (int phase{0}; phase < times; ++phase) {
    input.swap(calculated);
    for (size_t i{0}; i < len; ++i) {
      genSequence(seqBuf, len, i);
      int sum{0};
      for (size_t j{0}; j < len; ++j) {
        sum += input[j] * seqBuf[j];
      }
      calculated[i] = std::abs(sum % 10);
    }
  }

  for (size_t c{0}; c < len && c < 8; ++c) {
    result.push_back(calculated[c] + '0');
  }

  return result;
}

/* Part 2 works with nasty hack: message seems to lay within second half of input vector. 
   At that part, each position in the output vector equals the sum of the same position in the input vector and all the elements to the right.
 */



std::string day16_2(const std::string &line) {
  int offset = stoi(line.substr(0, 7));
  int sigLen = line.size() * 10000;
  int vectSize = sigLen - offset;

  std::vector<int> buff(vectSize);

  size_t lineIdx = offset % line.size();

  for (int i{0}; i < vectSize; ++i, ++lineIdx) {
    if (lineIdx >= line.size()) lineIdx = 0;
    buff[i] = line[lineIdx] - '0';
  }

  for (int n{0}; n < 100; ++n) {
    int sum{0};
    for (int i{vectSize - 1}; i >= 0; --i) {
      buff[i] = (sum + buff[i]) % 10;
      sum = buff[i];
    }
  }

  std::string result{""};

  for (int i{0}; i < 8; ++i) {
    result.push_back(buff[i] + '0');
  }

  return result;
}

void day16(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      runTest(day16_1("12345678", 1), "48226158");
      runTest(day16_1("12345678", 2), "34040438");
      runTest(day16_1("12345678", 3), "03415518");
      runTest(day16_1("12345678", 4), "01029498");

      runTest(day16_1("80871224585914546619083218645595"), "24176176");
      runTest(day16_1("19617804207202209144916044189917"), "73745418");
      runTest(day16_1("69317163492948606335995924319873"), "52432133");
    } else {
      runTest(day16_2("03036732577212944063491565474664"), "84462026");
      runTest(day16_2("02935109699940807407585447034323"), "78725270");
      runTest(day16_2("03081770884921959731165446850517"), "53553731");
    }

  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day16_1(slist.get(0)) << "\n";

      } else {
        std::cout << "result: " << day16_2(slist.get(0)) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}