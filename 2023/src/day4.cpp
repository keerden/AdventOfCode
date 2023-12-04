#include <bits/stdc++.h>

#include "solutions.h"



static long int checkGame(const std::string &line){
    std::unordered_set<int> winning_numbers {};

    std::stringstream numbers {line};
    std::string token;
    bool checkNumbers {false};
    long int score{0};
    while(std::getline(numbers, token, ' ')){
        if(token == ""){
            continue;
        }
        if(token == "|"){
            checkNumbers = true;
            continue;
        }
        int value {std::stoi(token)};


        if(checkNumbers){
            if(winning_numbers.count(value)){
                score++;
            }
        } else {
            winning_numbers.insert(value);
        }
    }

    return score;

}

int parseGameNum(const std::string &line){
    std::stringstream ss{line};
    std::string dummy;
    int result;

    ss >> dummy;
    ss >> result;

    return result;
}


int day4_1(const Stringlist &input) {
    int total{0};

    for (const std::string &line : input) {
        size_t posColon {line.find(":")};
        assert(posColon != std::string::npos);
        int gameResult = checkGame(line.substr(posColon + 2));
        if(gameResult){
            total += 1 << (gameResult - 1);
        }
    }

    return total;
}
long int day4_2(const Stringlist &input) {
    long int total{0};
    std::unordered_map<int, long int> cardCopies{};

    for (const std::string &line : input) {
        size_t posColon {line.find(":")};
        assert(posColon != std::string::npos);

        long int copies{0};
        int gameNum = parseGameNum(line.substr(0, posColon));
        int gameResult = checkGame(line.substr(posColon + 2));



        auto it = cardCopies.find(gameNum);
        if(it != cardCopies.end()){
            copies = it->second;
        }

        int won = 1 + copies;
        total += won;

        for(int i = gameNum + 1; i <= gameNum + gameResult; i++){
            it = cardCopies.find(gameNum);
            if(it == cardCopies.end()){
                cardCopies[i] = won;
            }else{
                cardCopies[i] += won;
            }
        }

    }

    return total;
}

void day4(int part, bool test, std::string filename) {
    Stringlist slist;
    if (test) {
        slist.fromString(
            "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53\n"
            "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19\n"
            "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1\n"
            "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83\n"
            "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36\n"
            "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11" 
        );
        if (part == 1) {

            runTest(day4_1(slist), 13);
        } else {
            runTest(day4_2(slist), 30);
        }
    } else {
        if (slist.fromFile(filename)) {
            if (part == 1) {
                std::cout << "result: " << day4_1(slist) << "\n";
            } else {
                std::cout << "result: " << day4_2(slist) << "\n";
            }

        } else {
            std::cout << "file not found!\n";
        }
    }
}
