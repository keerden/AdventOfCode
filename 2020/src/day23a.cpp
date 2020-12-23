#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

typedef struct cup_s {
  int value;
  cup_s* next;
  cup_s* prev;
} cup;

class CupCircle {
 private:
  cup* m_current;
  int m_maxCupVal;
  int m_minCupVal;
  cup* m_first;
  std::vector<cup*> m_lookup;

 public:
  CupCircle(std::string initial, size_t size) {
    if (initial.empty()) return;
    cup* newcup;
    m_lookup.resize(size + 1, nullptr);

    newcup = new cup;
    newcup->value = initial[0] - '0';
    newcup->next = newcup->prev = newcup;
    m_current = newcup;
    m_minCupVal = m_maxCupVal = newcup->value;
    m_first = m_current;
    m_lookup[newcup->value] = newcup;

    for (size_t i{1}; i < initial.length(); ++i) {
      newcup = createCup(initial[i] - '0', newcup);
    }

    for (size_t i{m_maxCupVal + 1}; i <= size; ++i) {
      newcup = createCup(i, newcup);
    }
  }

  void playRound(int num) {
    for (int i{0}; i < num; ++i) {
      playRound();
      if (i % 100'000 == 0) std::cout << i << "\n";
    }
  }

  void playRound() {
    cup* pickup[3];
    cup* destcup;
    bool found{false};
    int newval = m_current->value;

    //printCups();

    //std::cout << "picked";
    for (int i{0}; i < 3; ++i) {
      pickup[i] = removeAfter(m_current);
      //std::cout << " " << pickup[i]->value;
    }
    //std::cout << "\n";

    while (!found) {
      --newval;
      if (newval < m_minCupVal) {
        newval = m_maxCupVal;
      }
      destcup = findCup(newval);
      found = (destcup != nullptr);
    }

    //std::cout << "\n" << destcup->value << " = destination\n";

    for (int i{2}; i >= 0; --i) {
      insertAfter(pickup[i], destcup);
    }
    m_current = m_current->next;
  }

  ~CupCircle() {
    while (m_current->next != m_current) {
      cup* removed = removeAfter(m_current);
      delete removed;
    }
    delete m_current;
    m_current = nullptr;
  }

  void printCups() {
    cup* c = m_first;
    do {
      if (c == m_current) {
        std::cout << "(" << c->value << ") ";
      } else {
        std::cout << c->value << " ";
      }
      c = c->next;
    } while (c != m_first);
    std::cout << "\n";
  }

  void printSolution() {
    cup* one = findCup(1);
    assert(one != nullptr);

    long long val1, val2;
    long long result;

    cup* c = one->next;
    val1 = c->value;
    val2 = c->next->value;
    result = val1 * val2;
    std::cout << "val1: " << val1 << " val2 " << val2 << "\nresult: " << result
              << "\n";
  }

 private:
  void insertAfter(cup* newcup, cup* existing) {
    newcup->next = existing->next;
    newcup->prev = existing;
    existing->next = newcup;
    newcup->next->prev = newcup;
  }

  cup* removeAfter(cup* existing) {
    cup* removed = existing->next;
    if (removed == m_first) m_first = existing;

    existing->next = removed->next;
    removed->next->prev = existing;
    removed->next = removed->prev = nullptr;

    return removed;
  }

  cup* findCup(int value) {
    if (value >= m_lookup.size()) return nullptr;

    cup* c = m_lookup[value];

    if (c == nullptr) return nullptr;

    if (c->next == nullptr || c->prev == nullptr) return nullptr;

    return c;
  }

  cup* createCup(int value, cup* prev) {
    cup* newcup = new cup;
    newcup->value = value;
    insertAfter(newcup, prev);
    if (value < m_minCupVal) {
      m_minCupVal = value;
    }
    if (value > m_maxCupVal) {
      m_maxCupVal = value;
    }

    m_lookup[value] = newcup;
    return newcup;
  }
};  // namespace

}  // namespace

int main() {
  std::string line;
  std::ifstream file;

  //CupCircle circle("389125467", 1'000'000);
  CupCircle circle("643719258", 1'000'000);


  circle.playRound(10'000'000);
  circle.printSolution();

  return 0;
}