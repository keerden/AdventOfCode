#include <bits/stdc++.h>

#include "solutions.h"

static bool isDigit(char c){
    return c >= '0' && c <= '9';
}


static std::unordered_set<Position2D> parseSymbols(const Stringlist &input){
    std::unordered_set<Position2D> result;


    for(size_t y{0}; y < input.size(); y++){
        const std::string &line = input.get(y);
        for(size_t x{0}; x < line.length(); x++){
            char c = line[x];
            if(c != '.' && !isDigit(c)){
                result.emplace(x,y);
            }
        }
    }
    return result;
}

static std::unordered_set<Position2D> parseGears(const Stringlist &input){
    std::unordered_set<Position2D> result;


    for(size_t y{0}; y < input.size(); y++){
        const std::string &line = input.get(y);
        for(size_t x{0}; x < line.length(); x++){
            char c = line[x];
            if(c == '*'){
                result.emplace(x,y);
            }
        }
    }
    return result;
}

bool checkSymbol(int x, int y, const std::unordered_set<Position2D> &symbols){
    Position2D key{x,y};
    return symbols.count(key) > 0;
}

bool checkPartNum(int posX, int posY, int size, int maxX, int maxY, const std::unordered_set<Position2D> &symbols){
    int minX = (posX == 0)? 0 : posX - 1;
    int minY = (posY == 0)? 1 : posY - 1;
    maxX = std::min(maxX, posX + size);
    maxY = std::min(maxY, posY + 1);

    for(int y{minY}; y <= maxY + 1; y+= 2){
        for(int x{minX}; x <= maxX; x++){
            if(checkSymbol(x,y, symbols)){
                return true;
            }
            
        }
    }
    if(minX < posX){
        if(checkSymbol(minX,posY, symbols)){
            return true;
        }
    }
    if(maxX == (posX + size)){
         if(checkSymbol(maxX,posY, symbols)){
            return true;
        }       
    }
    return false;
}

int day3_1(const Stringlist &input) {
    long int total{0};

    std::unordered_set<Position2D> symbols = parseSymbols(input);
    int maxY = input.size();
    for (int y{0}; y < maxY; ++y) {
        const std::string &line = input.get(y);
        int maxX = line.size();
        bool parsingNumber = false;
        int startX;
        std::string number;
        for(int x{0}; x < maxX; ++x){
            if(isDigit(line[x])){
                if(parsingNumber){
                    number.push_back(line[x]);
                }else{
                    startX = x;
                    parsingNumber = true;
                    number = line[x];
                }
            }else{
                if(parsingNumber && checkPartNum(startX,y,number.length(),maxX, maxY, symbols)){
                    long int value = std::stol(number);
                    total += value;
                }
                parsingNumber = false;
            }
        }
        if(parsingNumber && checkPartNum(startX,y,number.length(),maxX, maxY, symbols)){
            long int value = std::stol(number);
            total += value;
        }
    }

    return total;
}

void addNumberToMap(std::string value, int startX, int y, std::unordered_map<Position2D, long int> &map){
    long int number = std::stol(value);
    for(size_t i = 0; i < value.length(); ++i){
        Position2D pos{startX + (int) i, y};
        map[pos] = number;
    }
}

long int checkGear(const Position2D &gearPos, const std::unordered_map<Position2D, long int> &numbers){
    std::set<long int> found;

    for(int y = gearPos.y-1; y < gearPos.y+2; y++){
        for(int x = gearPos.x-1; x < gearPos.x+2; x++){
            if(x == gearPos.x && y == gearPos.y)
                continue;
                
            auto it = numbers.find(Position2D(x,y));
            if(it == numbers.end())
                continue;
            
            if(found.contains(it->second)){
                continue;
            }

            if(found.size() >= 2)
                return 0;

            found.insert(it->second);
        }
    }

    if(found.size() == 2){
        auto it = found.begin();
        long int first = *it;
        it++;
        return *it * first;
    }
    return 0;
}


long int day3_2(const Stringlist &input) {
    long int total{0};

    std::unordered_set<Position2D> gears = parseGears(input);
    std::unordered_map<Position2D, long int> numbers {};
    int maxY = input.size();
    for (int y{0}; y < maxY; ++y) {
        const std::string &line = input.get(y);
        int maxX = line.size();
        bool parsingNumber = false;
        int startX;
        std::string number;
        for(int x{0}; x < maxX; ++x){
            if(isDigit(line[x])){
                if(parsingNumber){
                    number.push_back(line[x]);
                }else{
                    startX = x;
                    parsingNumber = true;
                    number = line[x];
                }
            } else{
                if(parsingNumber)
                    addNumberToMap(number, startX, y, numbers);

                parsingNumber = false;
            }
        }
        if(parsingNumber)
            addNumberToMap(number, startX, y, numbers);
    }

    for(const Position2D &gearPos : gears){
        total += checkGear(gearPos, numbers);
    }


    return total;
}

void day3(int part, bool test, std::string filename) {
    Stringlist slist;
    if (test) {
        slist.fromString(
            "467..114..\n"
            "...*......\n"
            "..35..633.\n"
            "......#...\n"
            "617*......\n"
            ".....+.58.\n"
            "..592.....\n"
            "......755.\n"
            "...$.*....\n"
            ".664.598.." 
        );
        if (part == 1) {

            runTest(day3_1(slist), 4361);
        } else {
            runTest(day3_2(slist), 467835);
        }
    } else {
        if (slist.fromFile(filename)) {
            if (part == 1) {
                std::cout << "result: " << day3_1(slist) << "\n";
            } else {
                std::cout << "result: " << day3_2(slist) << "\n";
            }

        } else {
            std::cout << "file not found!\n";
        }
    }
}
