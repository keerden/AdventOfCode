#include <iostream>
#include <unordered_map>
#include <vector>


using namespace std;

#define NUM_TURNS 30000000

int main()
{
  vector<int> input {14,8,16,0,1,17};
  unordered_map<int,int>numbers;
  unordered_map<int,int>::iterator it;
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
        //  cout << "not found";
        spoken = 0;
        numbers[prev_spoken] = turn - 1;
      }else{
     //   cout << " found: " << it->second;
        spoken = turn - 1 - it->second;
        it->second = turn - 1;
      }
    }
  //  cout << "  = " << spoken << "\n";
    prev_spoken = spoken;
  }


  cout << "result: " << spoken << endl;


  return 0;
}