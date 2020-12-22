#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

int main() {
  std::string line;
  std::ifstream file;
  std::list<int> player1;
  std::list<int> player2;
  long long result{0};
  int rounds{0};

  file.open("input/day22.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  while (getline(file, line) && line != "") {  // player1
     if(line == "Player 1:")
      continue;
    player1.push_back(std::stoi(line));
  }
  while (getline(file, line) ) {  // player2
     if(line == "Player 2:")
      continue;
    player2.push_back(std::stoi(line));
  }

  while(!player1.empty() && !player2.empty())
  {
    int card1, card2;
    card1 = player1.front();
    player1.pop_front();

    card2 = player2.front();
    player2.pop_front();

    if(card1 > card2){
      player1.push_back(card1);
      player1.push_back(card2);
    }else{
      player2.push_back(card2);
      player2.push_back(card1);
    }
    ++rounds;
  }

  if(!player1.empty()){
    int mul = player1.size();
    for(int card: player1){
      result += card * mul;
      --mul;
    }
  }else{
    int mul = player2.size();
    for(int card: player2){
      result += card * mul;
      --mul;
    }
  }

  std::cout << rounds << " rounds played, result: " << result << "\n";

  return 0;
}