#include "intcode.h"

Intcode::Intcode(std::vector<int> program) {
  reset(program);
}

Intcode::Intcode(const std::string &program) {
  reset(program);
}

void Intcode::reset(std::vector<int> program) {
  m_memory = std::move(program);
  m_ip = 0;
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
  while (getline(ss, val, ','))
    m_memory.push_back(stoi(val));
}

int Intcode::readMem(int addr) const {
  if (addr < 0 || (size_t)addr >= m_memory.size()) {
    std::cout << "ERROR, readMem(): address out of bounds!\n";
    return 0;
  }
  return m_memory[addr];
}

void Intcode::writeMem(int addr, int value) {
  if (addr < 0 || (size_t)addr >= m_memory.size()) {
    std::cout << "ERROR, writeMem(): address out of bounds!\n";
    return;
  }
  m_memory[addr] = value;
}

void Intcode::setAllInput(const std::list<int> &input) {
  m_input = std::move(input);
}

void Intcode::setInput(int value) {
  m_input.push_back(value);
}

const std::list<int> &Intcode::getAllOutput() {
  return std::move(m_output);
}

int Intcode::getOutput() {
  int result = m_output.front();
  m_output.pop_front();

  return result;
}

bool Intcode::hasOutput() const {
  return !m_output.empty();
}

void Intcode::printMemory() const {
  for (size_t i{0}; i < m_memory.size(); ++i) {
    std::cout << i << ": " << m_memory[i] << "\n";
  }
}

std::string Intcode::memToString() const {
  std::stringstream output{};
  bool first{true};

  for (int i : m_memory) {
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
    int isize{1};
    int opcode;
    int parmode1, parmode2, parmode3;

    //decode instruction
    int instruction{readMem(m_ip)};

    opcode = instruction % 100;
    instruction /= 100;
    parmode1 = instruction % 10;
    instruction /= 10;
    parmode2 = instruction % 10;
    instruction /= 10;
    parmode3 = instruction % 10;

    switch (opcode) {
      case 1:  //add
        writeMem(readMem(m_ip + 3), fetchParam(m_ip + 1, parmode1) + fetchParam(m_ip + 2, parmode2));
        isize = 4;
        break;

      case 2:  //mul
        writeMem(readMem(m_ip + 3), fetchParam(m_ip + 1, parmode1) * fetchParam(m_ip + 2, parmode2));
        isize = 4;
        break;

      case 3:  //read from input
        if (!hasInput()) {
          return false;
        } else {
          writeMem(readMem(m_ip + 1), getInput());
        }
        isize = 2;
        break;

      case 4:  //write to output
        m_output.push_back(fetchParam(m_ip + 1, parmode1));
        isize = 2;
        break;

      case 5:  //jump if true
        if (fetchParam(m_ip + 1, parmode1) != 0) {
          isize = 0;
          m_ip = fetchParam(m_ip + 2, parmode2);
        } else {
          isize = 3;
        }
        break;

      case 6:  //jump if false
        if (fetchParam(m_ip + 1, parmode1) == 0) {
          isize = 0;
          m_ip = fetchParam(m_ip + 2, parmode2);
        } else {
          isize = 3;
        }
        break;

      case 7:  //less than
        if (fetchParam(m_ip + 1, parmode1) < fetchParam(m_ip + 2, parmode2)) {
          writeMem(readMem(m_ip + 3), 1);
        } else {
          writeMem(readMem(m_ip + 3), 0);
        }
        isize = 4;
        break;

      case 8:  //equals
        if (fetchParam(m_ip + 1, parmode1) == fetchParam(m_ip + 2, parmode2)) {
          writeMem(readMem(m_ip + 3), 1);
        } else {
          writeMem(readMem(m_ip + 3), 0);
        }
        isize = 4;
        break;
      case 99:  //halt
        done = true;
        isize = 1;
        break;
      default:
        std::cout << "error!\n";
        return true;
        break;
    }
    m_ip += isize;
  }
  return true;
}

int Intcode::fetchParam(int ip, int mode) const {
  int result{0};
  switch (mode) {
    case 0:  //load from memory
      result = readMem(readMem(ip));
      break;
    case 1:  //immediate
      result = readMem(ip);
      break;
  }
  return result;
}

void Intcode::setOutput(int value){
  m_output.push_back(value);
}

int Intcode::getInput(){
  int result = m_input.front();
  m_input.pop_front();
  return result;
}

bool Intcode::hasInput() const{
  return !m_input.empty();
}
