#include <bits/stdc++.h>

#include "solutions.h"

std::unordered_map<char, unsigned int> cardValues =
{
    {'2', 0},
    {'3', 1},
    {'4', 2},
    {'5', 3},
    {'6', 4},
    {'7', 5},
    {'8', 6},
    {'9', 7},
    {'T', 8},
    {'J', 9},
    {'Q', 10},
    {'K', 11},
    {'A', 12},
};


std::unordered_map<char, unsigned int> cardValuesWithJoker =
{
    {'J', 0},
    {'2', 1},
    {'3', 2},
    {'4', 3},
    {'5', 4},
    {'6', 5},
    {'7', 6},
    {'8', 7},
    {'9', 8},
    {'T', 9},
    {'Q', 10},
    {'K', 11},
    {'A', 12},
};



class Hand {
    private:
        int cards[5];
        unsigned int totalValue;
        bool useJoker;

    public:
        Hand(std::string input, bool useJoker = false){
            this->useJoker = useJoker;
            assert(input.length() == 5);

            totalValue = 0;

            const std::unordered_map<char, unsigned int> &cardMap = useJoker ?  cardValuesWithJoker : cardValues;

            for(int i{0}; i < 5; i++){
                unsigned int value = cardMap.at(input[i]);
                cards[i] = value;
                totalValue <<= 4;
                totalValue += value;
            } 

            totalValue += (typeValue() << (5 * 4));
        }

        unsigned int getTotalValue(){
            return totalValue;
        }

    private:
        unsigned int typeValue(){
            std::unordered_map <unsigned int, int> distinctCards;
            int jokerCount{0};

            for(unsigned int card : cards){
                if(useJoker && card == cardValuesWithJoker['J']){
                    jokerCount++;
                    continue;
                }

                if(distinctCards.count(card)){
                    distinctCards[card]++;
                }else{
                    distinctCards[card] = 1;
                }
            }


            if(useJoker){
                if(distinctCards.size() == 0){
                    distinctCards.emplace(cardValuesWithJoker['A'], 5);
                } else {
                    unsigned int jokerValue;
                    int largestAmount {0};

                    for(auto &it : distinctCards){
                        if(it.second > largestAmount){
                            largestAmount = it.second;
                            jokerValue = it.first;
                            continue;
                        }

                        if(it.second == largestAmount && it.first > jokerValue){
                            jokerValue = it.first;
                        }
                    }

                    distinctCards[jokerValue] += jokerCount;
                }                
            }

            auto it = distinctCards.cbegin();
            switch (distinctCards.size()){
                case 1: //5 of a kind
                    return 7;

                case 2: //either 4 of a kind or full house
                    if(it->second == 4 || it->second  == 1){
                        return 6; //4 of a kind
                    }
                    return 5; //full house
                case 3: //3 of a kind or 2 pair
                    if(it->second == 3 || (++it)->second == 3 || (++it)->second == 3){
                        return 4; //3 of a kind
                    }
                    return 3; //2 pair
                case 4:
                    return 2; //1 pair
                case 5:
                    return 1; //High card
            }
            return 0;
        }
};




int playGame(const Stringlist &input, bool useJoker){
    std::multimap<unsigned long,int> rankedHands;

    for(const std::string &line : input){
        std::stringstream ss(line);
        std::string cards;
        int bid;

        ss >> cards;
        ss >> bid;
        Hand hand{cards, useJoker};
        rankedHands.emplace(hand.getTotalValue(), bid);
    }

    int result{0};
    int i{1};
    for(auto it = rankedHands.begin(); it != rankedHands.end(); it++){
        result += i * it->second;
        i++;
    }   
    return result;
}

int day7_1(const Stringlist &input) {
    return playGame(input, false);
}
long int day7_2(const Stringlist &input) {
    return playGame(input, true);
}

void day7(int part, bool test, std::string filename) {
    Stringlist slist;
    if (test) {
        slist.fromString(
            "32T3K 765\n"
            "T55J5 684\n"
            "KK677 28\n"
            "KTJJT 220\n"
            "QQQJA 483" 
        );
        if (part == 1) {
            runTest(day7_1(slist), 6440);
        } else {
            runTest(day7_2(slist), 5905);
        }
    } else {
        if (slist.fromFile(filename)) {
            if (part == 1) {
                std::cout << "result: " << day7_1(slist) << "\n";
            } else {
                std::cout << "result: " << day7_2(slist) << "\n";
            }

        } else {
            std::cout << "file not found!\n";
        }
    }
}
