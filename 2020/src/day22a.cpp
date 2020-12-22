#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>

// true if player 1 wins
bool playGame(std::list<int> &player1, std::list<int> &player2, int &game) {
  std::set<std::list<int>> prevdecks1;
  std::set<std::list<int>> prevdecks2;
  int rounds{1}, thisgame{game};
  bool player1Win;

  // std::cout << "===== game " << thisgame << " =====\n\n";

  while (!player1.empty() && !player2.empty()) {
    //   std::cout << "-- round " << rounds << "(" << thisgame << ") --\n";
    //   std::cout << "Player 1's deck:";
    //   for (auto c : player1) std::cout << " " << c;
    //   std::cout << "\nPlayer 2's deck:";
    //    for (auto c : player2) std::cout << " " << c;
    //  std::cout << "\n";

    if (prevdecks1.count(player1) > 0 || prevdecks2.count(player2) > 0) {
      //     std::cout << "same deck, player 1 wins\n";
      return true;
    }
    prevdecks1.insert(player1);
    prevdecks2.insert(player2);

    int card1, card2;
    card1 = player1.front();
    player1.pop_front();

    card2 = player2.front();
    player2.pop_front();

    //   std::cout << "Player 1 plays: " << card1 << "\nPlayer 2 plays: " <<
    //   card2 << "\n";

    if (player1.size() >= card1 && player2.size() >= card2) {
      // recurse
      std::list<int> newplayer1;
      std::list<int> newplayer2;
      int count{0};

      for (int c : player1) {
        newplayer1.push_back(c);
        ++count;
        if (count >= card1) break;
      }
      count = 0;
      for (int c : player2) {
        newplayer2.push_back(c);
        ++count;
        if (count >= card2) break;
      }
      //     std::cout << "Playing a sub-game to determine the winner...\n\n";
      player1Win = playGame(newplayer1, newplayer2, ++game);
      //    std::cout << "...anyway, back to game " << thisgame << "\n";

    } else {
      player1Win = (card1 > card2);
    }
    if (player1Win) {
      //      std::cout << "Player 1 wins round " << rounds << " of game " <<
      //      thisgame
      //                << "\n\n";
      player1.push_back(card1);
      player1.push_back(card2);
    } else {
      //      std::cout << "Player 2 wins round " << rounds << " of game " <<
      //      thisgame
      //                << "\n\n";
      player2.push_back(card2);
      player2.push_back(card1);
    }
    ++rounds;
  }

  //  std::cout << "The winner of game " << thisgame << " is player "
  //            << player1.empty() + 1 << "\n\n";
  return player2.empty();
}

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
    if (line == "Player 1:") continue;
    player1.push_back(std::stoi(line));
  }
  while (getline(file, line)) {  // player2
    if (line == "Player 2:") continue;
    player2.push_back(std::stoi(line));
  }
  int game{1};
  playGame(player1, player2, game);

  if (!player1.empty()) {
    int mul = player1.size();
    for (int card : player1) {
      result += card * mul;
      --mul;
    }
  } else {
    int mul = player2.size();
    for (int card : player2) {
      result += card * mul;
      --mul;
    }
  }

  std::cout << "result: " << result << "\n";

  return 0;
}