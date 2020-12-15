#include <iostream>
#include <map>
#include <vector>


using namespace std;

#define NUM_TURNS 30000000

int main()
{
  vector<int> input {14,8,16,0,1,17};
  map<int,int>numbers;
  map<int,int>::iterator it;
  int turn{0};
  int spoken{0};
  int prev_spoken{0};


  for(turn = 0; turn < NUM_TURNS; turn++){
  //  cout <<"#" << turn ;
    if(turn < input.size()){
      spoken = input[turn];
      numbers[spoken] = turn;
    }else{
      it = numbers.find(prev_spoken);
      if(it == numbers.end()){
        spoken = 0;
      //  cout << "not found";
      }else{
     //   cout << " found: " << it->second;
        spoken = turn - 1 - it->second;
      }
      numbers[prev_spoken] = turn - 1;
    }
  //  cout << "  = " << spoken << "\n";
    prev_spoken = spoken;
  }


  cout << "result: " << spoken << endl;


  return 0;
}