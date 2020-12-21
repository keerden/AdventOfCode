#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

using wordset_t = std::set<std::string>;
using ingrmap_t = std::unordered_map<std::string, std::pair<int, wordset_t>>;
using allergenmap_t = std::unordered_map<std::string, wordset_t>;

void incAppearance(ingrmap_t &map, const std::string &word) {
  auto it = map.find(word);
  if (it != map.end()) {
    it->second.first++;
  } else {
    map[word].first = 1;
  }
}

int main() {
  std::string line;
  std::ifstream file;
  ingrmap_t ingedients;
  allergenmap_t allergens;

  file.open("input/day21.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  while (getline(file, line)) {  // collect rules
    std::stringstream ss(line);
    std::string token;
    wordset_t words;
    while (ss >> token && token != "(contains") {  // find words
      words.insert(token);
      incAppearance(ingedients, token);
    }
    std::string allergen;
    while (ss >> allergen) {  // find allergens
      allergen.pop_back();
      auto it = allergens.find(allergen);
      if (it ==
          allergens
              .end()) {  // first time ingredient was found, just add whole set
        allergens[allergen] = words;
        for (auto &word : words) {
          ingedients[word].second.insert(allergen);
        }
      } else {
        wordset_t result;
        wordset_t &oldset = it->second;
        // intersect with words wit oldset set
        // remove allergen from ingredients not in intersection
        // add allergen to ingredients in intersection
        auto oldset_it = oldset.begin();
        auto words_it = words.begin();

        while (oldset_it != oldset.end() && words_it != words.end()) {
          if (*oldset_it < *words_it) {
            ingedients[*oldset_it].second.erase(allergen);
            ++oldset_it;
          } else if (*words_it < *oldset_it) {
            ingedients[*words_it].second.erase(allergen);
            ++words_it;
          } else {
            ingedients[*words_it].second.insert(allergen);
            result.insert(*words_it);
            ++oldset_it;
            ++words_it;
          }
        }

        // handle remainders
        while (oldset_it != oldset.end()) {
          ingedients[*oldset_it].second.erase(allergen);
          oldset_it++;
        }
        while (words_it != words.end()) {
          ingedients[*words_it].second.erase(allergen);
          words_it++;
        }

        oldset = std::move(result);
      }
    }
  }

  // remove 'solved' ingredients

  bool done{false};
  while (!done) {
    done = true;
    for (auto &ing : ingedients) {
      wordset_t &agen = ing.second.second;
      if (agen.size() == 1) {
        std::string solved_allergen = *agen.begin();
        if (allergens[solved_allergen].size() > 1) {
          done = false;
          auto it = allergens[solved_allergen].begin();
          while (it != allergens[solved_allergen].end()) {
            auto old_it = it;
            ++it;
            if (*old_it != ing.first) {
              ingedients[*old_it].second.erase(solved_allergen);
              allergens[solved_allergen].erase(old_it);
            }
          }
          break;
        }
      }
    }
  }
  std::list<std::pair<std::string, std::string>> solved;

  for (auto &ing : ingedients) {
    if (ing.second.second.size() == 1) {
      std::string ingname = ing.first;
      std::string allname = *ing.second.second.begin();

      auto it = solved.begin();
      while (it != solved.end() && it->second < allname) ++it;
      solved.insert(it, {ingname, allname});
    }
  }
  std::cout << "\nresult:\n";
  bool first{true};
  for (auto &p : solved) {
    if (!first) std::cout << ",";
    first = false;
    std::cout << p.first;
  }
  std::cout << "\n";

  return 0;
}