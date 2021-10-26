#include <bits/stdc++.h>

#include "util.h"

struct ingredient {
  int amount;
  std::string name;

  ingredient(int amount, std::string name) : amount{amount}, name{name} {};
};

typedef std::map<std::string, std::tuple<int, std::vector<struct ingredient>>> reactMap_t;

void fillMap(const Stringlist &slist, reactMap_t &reactions) {
  for (auto &line : slist) {
    std::vector<struct ingredient> iVect;
    std::stringstream ss(line);
    std::string token;
    bool arrow{false};
    int num{-1};

    while (std::getline(ss, token, ' ')) {
      if (token == "=>") {
        arrow = true;
        continue;
      }

      if (num == -1) {
        num = stoi(token);
      } else {
        if (token.back() == ',')
          token.resize(token.size() - 1);

        if (arrow) {
          reactions[token] = std::make_tuple(num, std::move(iVect));
        } else {
          iVect.emplace_back(num, token);
        }
        num = -1;
      }
    }
  }
}

void printMap(std::map<std::string, long long int> &residue) {
  std::cout << "map:\n";
  for (auto &m : residue) {
    std::cout << m.first << "=" << m.second << "\n";
  }
}

long long int calc(reactMap_t &rmap, std::map<std::string, long long int> &residue, const std::string &item, long long int needAmt) {
  long long int result{0};
  auto &tup = rmap[item];

  int amt = std::get<0>(tup);
  auto &iVect = std::get<1>(tup);
  long long int multiplier = (needAmt + (amt - 1)) / amt;
  long long int leftover = amt * multiplier - needAmt;

  //std::cout << "Producing " << item << " n:" << needAmt << " p:" << amt << " mul:" << multiplier << " rest:" << leftover << "\n";

  for (auto &i : iVect) {
    long long int needI = i.amount * multiplier;
    //std::cout << "\tneed " << needI << " " << i.name << "\n";

    if (i.name == "ORE") {
      result += needI;
    } else {
      long long int n = residue[i.name];
      //std::cout << "\tfound " << n << " " << i.name << "\n";
      if (n >= needI) {
        residue[i.name] -= needI;
        //std::cout << "\tleft " << residue[i.name] << " " << i.name << "\n";
      } else {
        needI -= n;
        residue[i.name] = 0;
        //std::cout << "\tstill needs " << needI << " " << i.name << "\n";
        result += calc(rmap, residue, i.name, needI);

        //std::cout << "left " << residue[i.name] << " " << i.name << "\n";
      }
    }
  }

  residue[item] += leftover;

  //std::cout << "----- Produced " << item << " ore: " << result << " leftover: " << residue[item] << "\n";

  return result;
}

long long int day14_1(const Stringlist &slist) {
  reactMap_t reactions;
  std::map<std::string, long long int> residue;
  fillMap(slist, reactions);

  return calc(reactions, residue, "FUEL", 1);
}

long long int fuelToOre(reactMap_t &reactions, long long int fuel) {
  std::map<std::string, long long int> residue;
  return calc(reactions, residue, "FUEL", fuel);
}

long long int day14_2(const Stringlist &slist) {
  reactMap_t reactions;
  fillMap(slist, reactions);
  long long int low = 1000000000000 / fuelToOre(reactions, 1);  //due to leftovers, lower bound on result
  long long int high = 10 * low;
  long long int oldmid{0};
  long long int mid{0};

  //std::cout << "starting: " << low << " " << high << "\n";
  //std::cout << fuelToOre(reactions, high) << "\n";
  while (fuelToOre(reactions, high) < 1000000000000) {
    low = high;
    high *= 10;
    //std::cout << "inrease: " << low << " " << high << "\n";
  }

  //at this point, result lays between low and high. try halfway and adjust bounds. repeat until found

  do {
    oldmid = mid;
    mid = low + (high - low) / 2;
    long long int midRes = fuelToOre(reactions, mid);

    if (midRes < 1000000000000) {  //too low, adjust bounds to higher half.
      low = mid;
      //std::cout << "select upper\n";
    } else if (midRes > 1000000000000) {  //too high, try lower half
      high = mid;
      //std::cout << "select lower\n";
    } else {  //found the exact answer, stop
      //std::cout << "stop!\n";
      return mid;
    }

    //std::cout << "mid: "<< mid << " = " << midRes << " new bounds: " << low << " - " << high << "\n";
  } while(mid != oldmid);

  //std::cout << "final: " << low << " " << high << "\n";

  return mid;
}

void day14(int part, bool test, std::string filename) {
  Stringlist slist;
  if (test) {
    if (part == 1) {
      slist.fromString(
          "10 ORE => 10 A\n"
          "1 ORE => 1 B\n"
          "7 A, 1 B => 1 C\n"
          "7 A, 1 C => 1 D\n"
          "7 A, 1 D => 1 E\n"
          "7 A, 1 E => 1 FUEL\n");

      runTest(day14_1(slist), 31);
      slist.fromString(
          "9 ORE => 2 A\n"
          "8 ORE => 3 B\n"
          "7 ORE => 5 C\n"
          "3 A, 4 B => 1 AB\n"
          "5 B, 7 C => 1 BC\n"
          "4 C, 1 A => 1 CA\n"
          "2 AB, 3 BC, 4 CA => 1 FUEL\n");
      runTest(day14_1(slist), 165);
      slist.fromString(
          "157 ORE => 5 NZVS\n"
          "165 ORE => 6 DCFZ\n"
          "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL\n"
          "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ\n"
          "179 ORE => 7 PSHF\n"
          "177 ORE => 5 HKGWZ\n"
          "7 DCFZ, 7 PSHF => 2 XJWVT\n"
          "165 ORE => 2 GPVTF\n"
          "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT");
      runTest(day14_1(slist), 13312);
      slist.fromString(
          "2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG\n"
          "17 NVRVD, 3 JNWZP => 8 VPVL\n"
          "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL\n"
          "22 VJHF, 37 MNCFX => 5 FWMGM\n"
          "139 ORE => 4 NVRVD\n"
          "144 ORE => 7 JNWZP\n"
          "5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC\n"
          "5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV\n"
          "145 ORE => 6 MNCFX\n"
          "1 NVRVD => 8 CXFTF\n"
          "1 VJHF, 6 MNCFX => 4 RFSQX\n"
          "176 ORE => 6 VJHF\n");
      runTest(day14_1(slist), 180697);
      slist.fromString(
          "171 ORE => 8 CNZTR\n"
          "7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL\n"
          "114 ORE => 4 BHXH\n"
          "14 VRPVC => 6 BMBT\n"
          "6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL\n"
          "6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT\n"
          "15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW\n"
          "13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW\n"
          "5 BMBT => 4 WPTQ\n"
          "189 ORE => 9 KTJDG\n"
          "1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP\n"
          "12 VRPVC, 27 CNZTR => 2 XDBXC\n"
          "15 KTJDG, 12 BHXH => 5 XCVML\n"
          "3 BHXH, 2 VRPVC => 7 MZWV\n"
          "121 ORE => 7 VRPVC\n"
          "7 XCVML => 6 RJRHP\n"
          "5 BHXH, 4 VRPVC => 5 LTCX\n");
      runTest(day14_1(slist), 2210736);
    } else {
      slist.fromString(
          "157 ORE => 5 NZVS\n"
          "165 ORE => 6 DCFZ\n"
          "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL\n"
          "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ\n"
          "179 ORE => 7 PSHF\n"
          "177 ORE => 5 HKGWZ\n"
          "7 DCFZ, 7 PSHF => 2 XJWVT\n"
          "165 ORE => 2 GPVTF\n"
          "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT");
      runTest(day14_2(slist), 82892753);
      slist.fromString(
          "2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG\n"
          "17 NVRVD, 3 JNWZP => 8 VPVL\n"
          "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL\n"
          "22 VJHF, 37 MNCFX => 5 FWMGM\n"
          "139 ORE => 4 NVRVD\n"
          "144 ORE => 7 JNWZP\n"
          "5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC\n"
          "5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV\n"
          "145 ORE => 6 MNCFX\n"
          "1 NVRVD => 8 CXFTF\n"
          "1 VJHF, 6 MNCFX => 4 RFSQX\n"
          "176 ORE => 6 VJHF\n");
      runTest(day14_2(slist), 5586022);
      slist.fromString(
          "171 ORE => 8 CNZTR\n"
          "7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL\n"
          "114 ORE => 4 BHXH\n"
          "14 VRPVC => 6 BMBT\n"
          "6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL\n"
          "6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT\n"
          "15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW\n"
          "13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW\n"
          "5 BMBT => 4 WPTQ\n"
          "189 ORE => 9 KTJDG\n"
          "1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP\n"
          "12 VRPVC, 27 CNZTR => 2 XDBXC\n"
          "15 KTJDG, 12 BHXH => 5 XCVML\n"
          "3 BHXH, 2 VRPVC => 7 MZWV\n"
          "121 ORE => 7 VRPVC\n"
          "7 XCVML => 6 RJRHP\n"
          "5 BHXH, 4 VRPVC => 5 LTCX\n");
      runTest(day14_2(slist), 460664);
    }

  } else {
    if (slist.fromFile(filename)) {
      if (part == 1) {
        std::cout << "result: " << day14_1(slist) << "\n";

      } else {
        std::cout << "result: " << day14_2(slist) << "\n";
      }

    } else {
      std::cout << "file not found!\n";
    }
  }
}