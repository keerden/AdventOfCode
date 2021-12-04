#include <bits/stdc++.h>

#include "solutions.h"

int day3_1(const Stringlist &input) {
  size_t binsize = input.get(0).length();
  std::vector<int> ones(binsize, 0);
  std::vector<int> zeros(binsize, 0);

  for (auto &line : input) {
    for (size_t i{0}; i < binsize; ++i) {
      if (line[i] == '1') {
        ++ones[i];
      } else {
        ++zeros[i];
      }
    }
  }

  int gamma{0}, epsilon{0};

  for (size_t i{0}; i < binsize; ++i) {
    gamma <<= 1;
    epsilon <<= 1;

    if (ones[i] > zeros[i]) {
      gamma |= 1;
    } else {
      epsilon |= 1;
    }
  }

  return gamma * epsilon;
}

int bitsToInt(const std::vector<bool> &vect) {
  int result{0};
  for (auto bit : vect) {
    result <<= 1;
    result |= bit ? 1 : 0;
  }
  return result;
}
int findByte(std::set<std::vector<bool>> data, bool least, std::vector<int> zeros, std::vector<int> ones){
  size_t binsize = zeros.size();
  for (size_t pos{0}; pos < binsize && data.size() > 1; ++pos) {
    auto itt = data.begin();
    bool common = (ones[pos] >= zeros[pos]) ^ least;
    while (itt != data.end()) {
      if ((*itt)[pos] != common) {
        for (size_t q{pos + 1}; q < binsize; ++q) {
          if ((*itt)[q]) {
            --ones[q];
          } else {
            --zeros[q];
          }
        }
        auto olditt = itt++;
        data.erase(olditt);
      } else {
        ++itt;
      }
    }
  }
  assert(data.size() == 1);

  return bitsToInt(*(data.begin()));
}

int day3_2(const Stringlist &input) {
  size_t binsize = input.get(0).length();
  std::set<std::vector<bool>> data;
  std::vector<int> ones(binsize, 0);
  std::vector<int> zeros(binsize, 0);

  for (auto &line : input) {
    std::vector<bool> bitvect;
    for (size_t i{0}; i < binsize; ++i) {
      if (line[i] == '1') {
        ++ones[i];
        bitvect.push_back(true);
      } else {
        ++zeros[i];
        bitvect.push_back(false);
      }
    }
    data.insert(bitvect);
  }

  int oxy = findByte(data, false, zeros, ones);
  int co2 = findByte(data, true, zeros, ones);

  return oxy * co2;
}

void day3(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    slist.fromString(
        "00100\n"
        "11110\n"
        "10110\n"
        "10111\n"
        "10101\n"
        "01111\n"
        "00111\n"
        "11100\n"
        "10000\n"
        "11001\n"
        "00010\n"
        "01010");
    if (part == 1) {
      runTest(day3_1(slist), 198);
    } else {
      runTest(day3_2(slist), 230);
    }
  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day3_1(slist) << "\n";
      } else {
        std::cout << "result: " << day3_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}