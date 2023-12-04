#include <bits/stdc++.h>

#include "solutions.h"

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

struct subset {
    int red;
    int green;
    int blue;

    bool isValid(){
        return red <= MAX_RED && green <= MAX_GREEN && blue <= MAX_BLUE;
    }

    void max(struct subset other){
        red = std::max(red, other.red);
        green = std::max(green, other.green);
        blue = std::max(blue, other.blue);
    }

};

static struct subset parseSubset(const std::string &input) {
    struct subset result = {0,0,0};
    std::stringstream buffer{input};
    std::string subsetString;
    while(std::getline(buffer, subsetString, ',')){
        std::stringstream tokens{subsetString};
        int value;
        tokens >> value;
        std::string color;
        tokens >> color;

        if(color == "red"){
            result.red += value;
        }
        if(color == "green"){
            result.green += value;
        }
        if(color == "blue"){
            result.blue += value;
        }

    }

    return result;
}

static int checkGame(const std::string &line) {
    std::string temp;
    std::size_t colon = line.find(":");
    int gameId;
    std::stringstream buffer {line.substr(0, colon)};
    buffer >> temp;
    buffer >> gameId;
    std::stringstream subsets{line.substr(colon + 1)};

    while(std::getline(subsets, temp, ';')){
        struct subset sub {parseSubset(temp)};
        if(!sub.isValid()){
            return 0;
        }
    }
    return gameId;
}

static long int minGame(const std::string &line) {
    std::string temp;
    std::size_t colon = line.find(":");
    std::stringstream subsets{line.substr(colon + 1)};
    struct subset result{0,0,0};

    while(std::getline(subsets, temp, ';')){
        struct subset sub {parseSubset(temp)};
        result.max(sub);
    }
    return result.red * result.green * result.blue;
}


int day2_1(const Stringlist &input) {
    int total = 0;
    for (const std::string &line : input) {
        total += checkGame(line);
    }

    return total;
}
long int day2_2(const Stringlist &input) {
    long int result = 0;

    for (const std::string &line : input) {
        result += minGame(line);
    }

    return result;
}

void day2(int part, bool test, std::string filename) {
    Stringlist slist;
    if (test) {
        slist.fromString(
            "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n"
            "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n"
            "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n"
            "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n"
            "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"
        );
        if (part == 1) {

            runTest(day2_1(slist), 8);
        } else {
            runTest(day2_2(slist), 2286);
        }
    } else {
        if (slist.fromFile(filename)) {
            if (part == 1) {
                std::cout << "result: " << day2_1(slist) << "\n";
            } else {
                std::cout << "result: " << day2_2(slist) << "\n";
            }

        } else {
            std::cout << "file not found!\n";
        }
    }
}
