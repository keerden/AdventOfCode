#include <iostream>
#include <fstream>
#include <vector>


using namespace std;
int main()
{
  ifstream file;
  int sum{0};
  int val, modfuel;

  file.open("input/day1.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }


  while(file >> val){
    modfuel = (val / 3) - 2;

    do{
      sum += modfuel;
      modfuel = (modfuel / 3) - 2;
    }while(modfuel > 0);

  }

  cout << "total: " << sum << endl;

  return 0;
}