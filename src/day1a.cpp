#include <iostream>
#include <fstream>
#include <vector>


using namespace std;
int main()
{
  vector<int> nums;
  ifstream file;
  int val;

  file.open("input/day1.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }


  while(file >> val){
    nums.push_back(val);
  }

  for(size_t i = 0; i < nums.size() - 2; i++){
    for(size_t j = i + 1; j < nums.size() - 1; j++){
      for(size_t k = j + 2; k < nums.size(); k++){
        if(nums.at(i) + nums.at(j) + nums.at(k) == 2020){
          cout << "answer: ";
          cout << nums.at(i) * nums.at(j) * nums.at(k);;
          cout << endl;
          return 0;
        }
      }
    }
  }

  cout << "answer not found :(" << endl;

  return 0;
}