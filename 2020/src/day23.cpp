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
  cup* m_maxCup;
  int m_minCupVal;
  cup* m_minCup;

 public:
  CupCircle(std::string initial) {
    if (initial.empty()) return;

    cup* inserted;
    cup* newcup;
    int value = initial[0] - '0';

    newcup = new cup;
    newcup->value = value;
    newcup->next = newcup->prev = newcup;
    m_current = newcup;
    m_minCupVal = m_maxCupVal = value;
    m_minCup = m_maxCup = m_current;
    inserted = newcup;

    for (size_t i{1}; i < initial.length(); ++i) {
      value = initial[i] - '0';
      newcup = new cup;
      newcup->value = value;
      insertAfter(newcup, inserted);
      if (value < m_minCupVal) {
        m_minCupVal = value;
        m_minCup = newcup;
      }
      if (value > m_maxCupVal) {
        m_maxCupVal = value;
        m_maxCup = newcup;
      }
      inserted = newcup;
    }
  }

  void playRound(int num) {
    for (int i{0}; i < num; ++i) playRound();
  }

  void playRound() {
    cup* pickup[3];
    cup* destcup;
    bool found{false};
    int newval = m_current->value;

    // printCups();

    // std::cout << "picked";
    for (int i{0}; i < 3; ++i) {
      pickup[i] = removeAfter(m_current);
      // std::cout << " " << pickup[i]->value ;
    }
    // std::cout << "\n";

    while (!found) {
      --newval;
      if (newval < m_minCupVal) {
        newval = m_maxCupVal;
        destcup = m_maxCup;
        found = destcup->next != nullptr;  // check if this item was not removed
      } else {
        destcup = findCup(newval);
        found = (destcup != nullptr);
      }
    }

    // std::cout << destcup->value << " = destination\n";

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
    std::cout << m_current->value;
    cup* c = m_current->next;
    while (c != m_current) {
      std::cout << " " << c->value;
      c = c->next;
    }
    std::cout << "\n";
  }

  void printSolution() {
    cup* one = findCup(1);
    assert(one != nullptr);

    cup* c = one->next;
    while (c != one) {
      std::cout << c->value;
      c = c->next;
    }
    std::cout << "\n";
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
    existing->next = removed->next;
    removed->next->prev = existing;
    removed->next = removed->prev = nullptr;

    return removed;
  }

  cup* findCup(int value) {
    cup* c = m_current;

    do {
      if (c->value == value) return c;
      c = c->next;

    } while (c != m_current);

    return nullptr;
  }
};

}  // namespace

int main() {
  std::string line;
  std::ifstream file;

  // CupCircle circle("389125467");
  CupCircle circle("643719258");

  circle.playRound(100);

  circle.printSolution();

  return 0;
}