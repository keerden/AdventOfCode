#ifndef _INTCODE_H
#define _INTCODE_H
#include <bits/stdc++.h>

using ic_word_t = int64_t;

class Intcode {

 private:
  std::vector<ic_word_t> m_memory;
  std::list<ic_word_t> m_input;
  std::list<ic_word_t> m_output;
  size_t m_ip;
  ic_word_t m_rel_base;
  

public:
  Intcode(std::vector<ic_word_t> program);
  Intcode(const std::string &program);

  void reset(std::vector<ic_word_t> program);
  void reset(const std::string &program);

  ic_word_t readMem(ic_word_t addr);
  void writeMem(ic_word_t addr, ic_word_t value);

  void setAllInput(const std::list<ic_word_t> &input);
  void setInput(ic_word_t value);

  const std::list<ic_word_t> &getAllOutput();
  ic_word_t getOutput();
  std::string outputToString() const;
  bool hasOutput() const;

  void printMemory() const;
  std::string memToString() const;

  bool run();

private:
  ic_word_t fetch(ic_word_t ip, int mode = 0);
  void store(ic_word_t value, ic_word_t ip, int mode = 0);

  void setOutput(ic_word_t value);
  ic_word_t getInput();
  bool hasInput() const;
};

#endif