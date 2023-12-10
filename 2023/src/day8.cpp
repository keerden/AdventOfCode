#include <bits/stdc++.h>

#include "solutions.h"

class Node {
    private:
        std::string name;
        std::string leftName;
        std::string rightName;
        Node* left;
        Node* right;

    public:
        Node(const std::string &name, const std::string &leftName, const std::string &rightName) : name{name}, leftName{leftName}, rightName{rightName}, left{NULL}, right{NULL} {};

        void populate(std::unordered_map<std::string, Node> &nodeMap){
            left = &nodeMap.at(leftName);
            right = &nodeMap.at(rightName);
        }

        std::string getName(){
            return name;
        }

        Node* getLeft(){
            return left;
        }

        Node* getRight(){
            return right;
        }
};

std::unordered_map <std::string, Node> generateMap(const Stringlist &input, Stringlist::const_iterator begin){
    std::unordered_map <std::string, Node> nodeMap;

    std::regex re{"^(\\w+) = \\((\\w+), (\\w+)\\)$"};

    for(auto it = begin; it != input.end(); it++){
        std::smatch sm;
        std::regex_match(*it, sm, re);
        assert(sm.size() == 4);

        nodeMap.emplace(sm[1], Node(sm[1], sm[2], sm[3]));
    }

    for(auto &item : nodeMap){
        item.second.populate(nodeMap);
    }

    return nodeMap;
}

int walkRoute(std::string route, std::unordered_map <std::string, Node> &nodeMap){
    const std::string startNode {"AAA"};
    const std::string endNode {"ZZZ"};
    int steps{0};

    size_t routeStep{0};
    Node *currentNode = &(nodeMap.at(startNode));
    
    do {
        currentNode = ((route[routeStep] == 'L') ? currentNode->getLeft() : currentNode->getRight());
        steps++;
        routeStep++;
        if(routeStep >= route.size()){
            routeStep = 0;
        }
    } while (currentNode->getName() != endNode && steps < 99999);

    return steps;
}

long int walkSingleRoute(std::string route, Node* node){
    long int steps{0};

    size_t routeStep{0};
    Node *currentNode = node;
    
    do {
        currentNode = ((route[routeStep] == 'L') ? currentNode->getLeft() : currentNode->getRight());
        steps++;
        routeStep++;
        if(routeStep >= route.size()){
            routeStep = 0;
        }
    } while (currentNode->getName().back() != 'Z');

    return steps;
}

long int walkMultipleRoute(std::string route, const std::vector<Node*> &startNodes){
    std::vector<long int> allSteps{};

    std::vector<Node*> currentNodes{startNodes};

    for(Node* node: currentNodes){
        allSteps.push_back(walkSingleRoute(route, node));
    }

    long int count = 1;

    for(size_t i = 0; i < allSteps.size(); i++){
        count = std::lcm(count, allSteps[i]);
    }


    return count;

}



int day8_1(const Stringlist &input) {

    assert(input.size() > 2);

    auto it = input.begin();

    std::string route = *it;
    it++;
    assert(*it == "");
    it++;

    auto nodeMap = generateMap(input, it);
    
    return walkRoute(route, nodeMap);
}
long int day8_2(const Stringlist &input) {
    assert(input.size() > 2);

    auto it = input.begin();

    std::string route = *it;
    it++;
    assert(*it == "");
    it++;

    auto nodeMap = generateMap(input, it);

    std::vector<Node*> startNodes{};
    for(auto &item : nodeMap){
        if(item.first.back() == 'A'){
            startNodes.push_back(&item.second);
        }
    }

    return walkMultipleRoute(route, startNodes);
}

void day8(int part, bool test, std::string filename) {
    Stringlist slist, slist2;
    if (test) {
        if (part == 1) {
            slist.fromString(
                "RL\n"
                "\n"
                "AAA = (BBB, CCC)\n"
                "BBB = (DDD, EEE)\n"
                "CCC = (ZZZ, GGG)\n"
                "DDD = (DDD, DDD)\n"
                "EEE = (EEE, EEE)\n"
                "GGG = (GGG, GGG)\n"
                "ZZZ = (ZZZ, ZZZ)" 
            );

            slist2.fromString(
                "LLR\n"
                "\n"
                "AAA = (BBB, BBB)\n"
                "BBB = (AAA, ZZZ)\n"
                "ZZZ = (ZZZ, ZZZ)" 
            );
        
            runTest(day8_1(slist), 2);
            runTest(day8_1(slist2), 6);
        } else {

            slist.fromString(
                "LR\n"
                "\n"
                "11A = (11B, XXX)\n" 
                "11B = (XXX, 11Z)\n" 
                "11Z = (11B, XXX)\n" 
                "22A = (22B, XXX)\n" 
                "22B = (22C, 22C)\n" 
                "22C = (22Z, 22Z)\n" 
                "22Z = (22B, 22B)\n" 
                "XXX = (XXX, XXX)" 
            );

            runTest(day8_2(slist), 6);
        }
    } else {
        if (slist.fromFile(filename)) {
            if (part == 1) {
                std::cout << "result: " << day8_1(slist) << "\n";
            } else {
                std::cout << "result: " << day8_2(slist) << "\n";
            }

        } else {
            std::cout << "file not found!\n";
        }
    }
}
