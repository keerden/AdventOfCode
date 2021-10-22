#include "intcode.h"

Intcode::Intcode(std::vector<ic_word_t> program) {
  reset(program);
}

Intcode::Intcode(const std::string &program) {
  reset(program);
}

void Intcode::reset(std::vector<ic_word_t> program) {
  m_memory = std::move(program);
  m_ip = 0;
  m_rel_base = 0;
  m_output.clear();
  m_input.clear();
}

void Intcode::reset(const std::string &program) {
  std::stringstream ss{program};
  std::string val;
  m_memory.clear();
  m_output.clear();
  m_input.clear();
  m_ip = 0;
  m_rel_base = 0;
  while (getline(ss, val, ','))
    m_memory.push_back(stoll(val));
}

ic_word_t Intcode::readMem(ic_word_t addr) {
  if (addr < 0) {
    std::cout << "ERROR, readMem(): negative address!\n";
    return 0;
  }
  if ((size_t)addr >= m_memory.size()) {
    m_memory.resize(addr + 1, 0);
  }
  return m_memory[addr];
}

void Intcode::writeMem(ic_word_t addr, ic_word_t value) {
  if (addr < 0) {
    std::cout << "ERROR, writeMem(): negative address!\n";
    return;
  }
  if ((size_t)addr >= m_memory.size()) {
    m_memory.resize(addr + 1, 0);
  }
  m_memory[addr] = value;
}

void Intcode::setAllInput(const std::list<ic_word_t> &input) {
  m_input = std::move(input);
}

void Intcode::setInput(ic_word_t value) {
  m_input.push_back(value);
}

const std::list<ic_word_t> &Intcode::getAllOutput() {
  return std::move(m_output);
}

ic_word_t Intcode::getOutput() {
  ic_word_t result = m_output.front();
  m_output.pop_front();

  return result;
}

std::string Intcode::outputToString() const {
  std::stringstream output{};
  bool first{true};

  for (ic_word_t i : m_output) {
    if (first) {
      first = false;
    } else {
      output << ",";
    }

    output << i;
  }

  return output.str();
}

bool Intcode::hasOutput() const {
  return !m_output.empty();
}

size_t Intcode::outputSize() const {
  return m_output.size();
}

void Intcode::printMemory() const {
  for (size_t i{0}; i < m_memory.size(); ++i) {
    std::cout << i << ": " << m_memory[i] << "\n";
  }
}

std::string Intcode::memToString() const {
  std::stringstream output{};
  bool first{true};

  for (ic_word_t i : m_memory) {
    if (first) {
      first = false;
    } else {
      output << ",";
    }

    output << i;
  }

  return output.str();
}

bool Intcode::run() {
  bool done{false};
  while (m_ip < m_memory.size() && !done) {
    ic_word_t isize{1};
    ic_word_t opcode;
    ic_word_t parmode1, parmode2, parmode3;

    //decode instruction
    ic_word_t instruction{readMem(m_ip)};

    opcode = instruction % 100;
    instruction /= 100;
    parmode1 = instruction % 10;
    instruction /= 10;
    parmode2 = instruction % 10;
    instruction /= 10;
    parmode3 = instruction % 10;

    switch (opcode) {
      case 1:  //add
        store(fetch(m_ip + 1, parmode1) + fetch(m_ip + 2, parmode2), m_ip + 3, parmode3);
        isize = 4;
        break;

      case 2:  //mul
        store(fetch(m_ip + 1, parmode1) * fetch(m_ip + 2, parmode2), m_ip + 3, parmode3);
        isize = 4;
        break;

      case 3:  //read from input
        if (!hasInput()) {
          return false;
        } else {
          store(getInput(), m_ip + 1, parmode1);
        }
        isize = 2;
        break;

      case 4:  //write to output
        m_output.push_back(fetch(m_ip + 1, parmode1));
        isize = 2;
        break;

      case 5:  //jump if true
        if (fetch(m_ip + 1, parmode1) != 0) {
          isize = 0;
          m_ip = fetch(m_ip + 2, parmode2);
        } else {
          isize = 3;
        }
        break;

      case 6:  //jump if false
        if (fetch(m_ip + 1, parmode1) == 0) {
          isize = 0;
          m_ip = fetch(m_ip + 2, parmode2);
        } else {
          isize = 3;
        }
        break;

      case 7:  //less than
        if (fetch(m_ip + 1, parmode1) < fetch(m_ip + 2, parmode2)) {
          store(1, m_ip + 3, parmode3);
        } else {
          store(0, m_ip + 3, parmode3);
        }
        isize = 4;
        break;

      case 8:  //equals
        if (fetch(m_ip + 1, parmode1) == fetch(m_ip + 2, parmode2)) {
          store(1, m_ip + 3, parmode3);
        } else {
          store(0, m_ip + 3, parmode3);
        }
        isize = 4;
        break;
      case 9:  //adjust relative base
        m_rel_base += fetch(m_ip + 1, parmode1);
        isize = 2;
        break;

      case 99:  //halt
        done = true;
        isize = 1;
        break;
      default:
        std::cout << "error! unknown opcode: " << opcode << "\n";
        return true;
        break;
    }
    m_ip += isize;
  }
  return true;
}

ic_word_t Intcode::fetch(ic_word_t ip, int mode) {
  ic_word_t result{0};
  switch (mode) {
    case 0:  //load from memory
      result = readMem(readMem(ip));
      break;
    case 1:  //immediate
      result = readMem(ip);
      break;
    case 2:  //immediate
      result = readMem(readMem(ip) + m_rel_base);
      break;
  }
  return result;
}

void Intcode::store(ic_word_t value, ic_word_t ip, int mode) {
  switch (mode) {
    case 0:  //load from memory
      writeMem(readMem(ip), value);
      break;
    case 1:  //immediate
      std::cout << "error! unable to store at immediate parameter\n";
      break;
    case 2:  //immediate
      writeMem(readMem(ip) + m_rel_base, value);
      break;
  }
}

void Intcode::setOutput(ic_word_t value) {
  m_output.push_back(value);
}

ic_word_t Intcode::getInput() {
  ic_word_t result = m_input.front();
  m_input.pop_front();
  return result;
}

bool Intcode::hasInput() const {
  return !m_input.empty();
}
