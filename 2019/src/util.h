#ifndef _UTIL_H
#define _UTIL_H
#include <bits/stdc++.h>

class Stringlist {
 public:
  bool fromFile(std::string filename);
  Stringlist &fromString(const std::string &input);
  const std::vector<std::string> &get() const;
  const std::string &get(size_t index) const;

  std::vector<std::string>::const_iterator begin() const;
  std::vector<std::string>::const_iterator end() const;

 private:
  std::vector<std::string> m_vect;
};

template <typename TResult1, typename TResult2>
bool runTest(TResult1 result, TResult2 expected, std::string testname = "") {
  std::cout << testname;
  if (result == expected) {
    std::cout << "\tOK\n";
  } else {
    std::cout << "\tFAIL expected:" << expected << " was:" << result << "\n";
  }

  return (result == expected);
}

#endif