#include <bits/stdc++.h>

#include "solutions.h"

class RangeItem {
  private:
    long int max;
    long int diff;

    long int source;
    long int destination;
    long int length;

  public:
    RangeItem(std::string line) {
        std::stringstream ss{line};
        ss >> destination;
        ss >> source;
        ss >> length;
        calcDiff();
    }
    RangeItem(void) : source{0}, destination{0}, length{0} { calcDiff(); }
    RangeItem(long int s, long int d, long int l) : source{s}, destination{d}, length{l} { calcDiff(); }
    RangeItem(long int min, long int max) : max{max}, diff{0}, source{min}, destination{min}, length{max - min} {};


    friend std::ostream &operator<<(std::ostream &output, const RangeItem &rm) {
        output << "(" << rm.source << "-->" << rm.destination << "[" << rm.length << "])";
        return output;
    }

    bool operator<(const RangeItem &rhs) const {
        return (source < rhs.source) || ((source == rhs.source) && (length < rhs.length));
    }

    long int convert(long int input) const {
        if (input < source || input > max){
            return 0;
        }
        return input + diff;
    }

    long int getMin() const {
        return source;
    }

    long int getMax() const {
        return max;
    }

    long int getDiff() const {
        return diff;
    }

  private:
    void calcDiff() {
        max = source + length;
        diff = destination - source;
    }
};

class RangeMap {
  private:
    std::list<RangeItem> ranges;
    bool sorted = false;

  public:
    RangeMap(){};

    void addMapping(const std::string &mapping) {
        ranges.emplace_back(mapping);
        sorted = false;
    }

    void sort() {
        ranges.sort();
        sorted = true;
    }

    long int mapValue(long int value) {
        if(!sorted)
            sort();
        
        for(const RangeItem &range : ranges){
            if(value < range.getMin())
                return value;

            if(value < range.getMax())
                return range.convert(value);
        }

        return value;
    }

    void mapArray(std::vector<long int> &values){
        for(size_t i = 0; i < values.size(); ++i){
            values[i] = mapValue(values[i]);
        }
    }

    void mapRanges(std::vector<RangeItem> &values){
        std::vector<RangeItem> newValues;
        if(!sorted)
            sort();

        for(size_t i = 0; i < values.size(); ++i){
            long int min = values[i].getMin();
            long int max = values[i].getMax();
            for(const RangeItem &range : ranges){
                if(min < range.getMin()){
                    if(max <= range.getMin()){
                        break;
                    }
                    //split seeds into unmapped part
                    newValues.emplace_back(min, range.getMin());
                    min = range.getMin();
                }

                if(min < range.getMax()){

                    if(max < range.getMax()){
                        min += range.getDiff();
                        max += range.getDiff();
                        break;
                    }

                    //max > range, split
                    newValues.emplace_back(range.convert(min), range.getMax() + range.getDiff());
                    min = range.getMax();                   
                }
            }
            newValues.emplace_back(min, max);
        }

        values = std::move(newValues);
    }
};

std::vector<long int> parseSeeds(std::string input){
    std::vector<long int> result;
    std::stringstream ss{input};
    std::string token;

    while(getline(ss, token, ' ')){
        if(token == "seeds:")
            continue;
        result.push_back(std::stol(token));
    }
    return result;
}

std::vector<RangeItem> parseSeedRanges(std::string input){
    std::vector<RangeItem> result;
    std::stringstream ss{input};
    std::string token;

    long int first;
    bool isFirst = true;
    while(getline(ss, token, ' ')){
        if(token == "seeds:")
            continue;


        if(isFirst){
            isFirst = false;
            first = std::stol(token);
            continue;
        }
        result.emplace_back(first, first, std::stol(token));
        isFirst = true;
    }
    return result;
}

int mapSeeds(const Stringlist &input){
    std::vector<long int> seeds = parseSeeds(input.get(0));
    std::vector<RangeMap> maps {};
    
    for(size_t i = 1; i < input.size(); ++i){
        if(input.get(i) == ""){
            maps.push_back(RangeMap());
            i++;
            continue;
        }
        maps.back().addMapping(input.get(i));
    }

    for(auto &map: maps){
        map.mapArray(seeds);
    }

    long int result = LONG_MAX;
    for(long int seed : seeds){
        result = std::min(result, seed);
    }
    return result;
}

int mapSeedRanges(const Stringlist &input){
    std::vector<RangeItem> seeds = parseSeedRanges(input.get(0));
    std::vector<RangeMap> maps {};
    
    for(size_t i = 1; i < input.size(); ++i){
        if(input.get(i) == ""){
            maps.push_back(RangeMap());
            i++;
            continue;
        }
        maps.back().addMapping(input.get(i));
    }

    for(auto &map: maps){
        map.mapRanges(seeds);
    }

    long int result = LONG_MAX;
    for(auto &seedRange : seeds){
        result = std::min(result, seedRange.getMin());
    }
    return result;
}

int day5_1(const Stringlist &input) {
    return mapSeeds(input);
}
long int day5_2(const Stringlist &input) {
    return mapSeedRanges(input);
}

void day5(int part, bool test, std::string filename) {
    Stringlist slist;
    if (test) {
        slist.fromString(
            "seeds: 79 14 55 13\n" 
            "\n"
            "seed-to-soil map:\n"
            "50 98 2\n"
            "52 50 48\n"
            "\n"
            "soil-to-fertilizer map:\n"
            "0 15 37\n"
            "37 52 2\n"
            "39 0 15\n"
            "\n"
            "fertilizer-to-water map:\n"
            "49 53 8\n"
            "0 11 42\n"
            "42 0 7\n"
            "57 7 4\n"
            "\n"
            "water-to-light map:\n"
            "88 18 7\n"
            "18 25 70\n"
            "\n"
            "light-to-temperature map:\n"
            "45 77 23\n"
            "81 45 19\n"
            "68 64 13\n"
            "\n"
            "temperature-to-humidity map:\n"
            "0 69 1\n"
            "1 0 69\n"
            "\n"
            "humidity-to-location map:\n"
            "60 56 37\n"
            "56 93 4" 
        );
        if (part == 1) {
            runTest(day5_1(slist), 35);
        } else {
            runTest(day5_2(slist), 46);
        }
    } else {
        if (slist.fromFile(filename)) {
            if (part == 1) {
                std::cout << "result: " << day5_1(slist) << "\n";
            } else {
                std::cout << "result: " << day5_2(slist) << "\n";
            }

        } else {
            std::cout << "file not found!\n";
        }
    }
}
