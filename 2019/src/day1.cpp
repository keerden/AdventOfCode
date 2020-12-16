#include <iostream>
#include <fstream>
#include <vector>


using namespace std;
int main()
{
  ifstream file;
  int sum{0};
  int val;

  file.open("input/day1.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }


  while(file >> val){
    sum += (val / 3) - 2;
  }

  cout << "total: " << sum << endl;

  return 0;
}