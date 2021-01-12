#ifndef _INTCODE_H
#define _INTCODE_H
#include <bits/stdc++.h>

class Intcode {
 private:
  std::vector<int> m_memory;
  std::list<int> m_input;
  std::list<int> m_output;
  size_t m_ip;
  

public:
  Intcode(std::vector<int> program);
  Intcode(const std::string &program);

  void reset(std::vector<int> program);
  void reset(const std::string &program);

  int readMem(int addr) const;
  void writeMem(int addr, int value);

  void setAllInput(const std::list<int> &input);
  void setInput(int value);

  const std::list<int> &getAllOutput();
  int getOutput();
  bool hasOutput() const;

  void printMemory() const;
  std::string memToString() const;

  bool run();

private:
  int fetchParam(int ip, int mode = 0) const;

  void setOutput(int value);
  int getInput();
  bool hasInput() const;
};

#endif