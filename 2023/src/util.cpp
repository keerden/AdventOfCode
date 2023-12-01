#include "util.h"

#include <bits/stdc++.h>

bool Stringlist::fromFile(std::string filename) {
  std::ifstream file;
  std::string line;
  file.open(filename);

  if (!file.is_open())
    return false;

  m_vect.clear();
  while (getline(file, line))
    m_vect.push_back(line);
  return true;
}

Stringlist &Stringlist::fromString(const std::string &input){
  std::stringstream ss{input};
  std::string line;
  m_vect.clear();
  while(getline(ss, line))
    m_vect.push_back(line);
  return *this;
}


const std::vector<std::string> &Stringlist::get() const {
  return m_vect;
}

const std::string &Stringlist::get(size_t index) const{
  return m_vect[index];
}

std::vector<std::string>::const_iterator Stringlist::begin() const {
  return m_vect.begin();
}
std::vector<std::string>::const_iterator Stringlist::end() const {
  return m_vect.end();
}

size_t Stringlist::size(void) const {
  return m_vect.size();
}
