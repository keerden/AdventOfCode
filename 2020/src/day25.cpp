#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

constexpr int subjectNr {7};

int bruteForceKey(long long int pubkey){
  int loops{0};
  long long int transform{1};
  do {
    transform *= subjectNr;
    transform = transform % 20201227;
    ++loops;
  }while(transform != pubkey);

  return loops;
}

long long int transformKey(long long int key, int loopsize){
  long long int transform{1};
  for(int i{0}; i < loopsize; ++i){
    transform *= key;
    transform = transform % 20201227;
  }
  return transform;
}

int main() {
   std::ifstream file;
  long long int pubkeyDoor{0};
  long long int pubkeyCard{0};
  int loopDoor{0};
  int loopCard{0};
  long long int encryptionKey{0};

 
  
  file.open("input/day25.txt");

  if (!file.is_open()) {
    std::cout << "unable to open file!\n";
    return 0;
  }

  file >> pubkeyCard;
  file >> pubkeyDoor;

  loopDoor = bruteForceKey(pubkeyDoor);
  loopCard = bruteForceKey(pubkeyCard);

  std::cout << "loopsize Door = " << loopDoor << " Loopsize Card = " << loopCard << "\n";
  
  encryptionKey = transformKey(pubkeyDoor, loopCard);

  std::cout << encryptionKey<< " is the encryptionkey\n";

  encryptionKey = transformKey(pubkeyCard, loopDoor);

  std::cout << encryptionKey<< " is the encryptionkey\n";

  return 0;
}