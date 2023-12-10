#include <bits/stdc++.h>

#include "solutions.h"

bool derriveSerie(std::vector<long int> &newSerie, const std::vector<long int> &oldSerie){
    bool nonZero{false};

    for(size_t i = 1; i < oldSerie.size(); i++){
        long int diff = oldSerie[i] - oldSerie[i-1];
        newSerie.push_back(diff);

        nonZero = nonZero || diff;
    }
    return nonZero;
} 

long int predictNext(const std::vector<long int> &serie){
    std::vector<std::vector<long int>> derrivatives{};
    derrivatives.push_back(serie);

    bool nonZero{true};

    while(nonZero){
        std::vector<long int> derrivative;
        nonZero = derriveSerie(derrivative, derrivatives.back());
        derrivatives.push_back(std::move(derrivative));
    }

    long int newval{0};
    for(int i = derrivatives.size() - 2; i >= 0; i--){
        newval += derrivatives[i].back();
    }


    return newval;
}

long int predictPrevious(const std::vector<long int> &serie){
    std::vector<std::vector<long int>> derrivatives{};
    derrivatives.push_back(serie);

    bool nonZero{true};

    while(nonZero){
        std::vector<long int> derrivative;
        nonZero = derriveSerie(derrivative, derrivatives.back());
        derrivatives.push_back(std::move(derrivative));
    }

    long int newval{0};
    for(int i = derrivatives.size() - 2; i >= 0; i--){
        newval = derrivatives[i].front() - newval;
    }


    return newval;
}


std::vector<long int> parseSerie(const std::string &line){
    std::vector<long int> result{};
    std::stringstream ss{line};
    long int value;
    while(ss >> value){
        result.push_back(value);
    }
    return result;
}


long int day9_1(const Stringlist &input) {
    long int sum{0};

    for(auto &line : input){
        std::vector<long int> serie = parseSerie(line);

        long int val = predictNext(serie);
        sum += val;
    }


    return sum;
}
long int day9_2(const Stringlist &input) {
    long int sum{0};

    for(auto &line : input){
        std::vector<long int> serie = parseSerie(line);

        long int val = predictPrevious(serie);
        sum += val;
    }


    return sum;
}

void day9(int part, bool test, std::string filename) {
    Stringlist slist;
    if (test) {
        slist.fromString(
            "0 3 6 9 12 15\n"
            "1 3 6 10 15 21\n"
            "10 13 16 21 30 45"
        );
        if (part == 1) {
            runTest(day9_1(slist), 114);
        } else {
            runTest(day9_2(slist), 2);
        }
    } else {
        if (slist.fromFile(filename)) {
            if (part == 1) {
                std::cout << "result: " << day9_1(slist) << "\n";
            } else {
                std::cout << "result: " << day9_2(slist) << "\n";
            }

        } else {
            std::cout << "file not found!\n";
        }
    }
}
