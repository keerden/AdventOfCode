#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
int main()
{
  string line;
  ifstream file;
  vector <string> map;
  long long result = 1;
  
  int right[5]  = {1, 3, 5, 7, 1};
  int down[5]   = {1, 1, 1, 1, 2};
  size_t width, height;
  
  file.open("input/day3.txt");

  if(!file.is_open()){
    cout << "unable to open file!" << endl;
    return 0;
  }

  while(getline(file, line))
    map.push_back(line);
  
  height = map.size();
  width = map[0].size();

  for(int i = 0; i < 5; i++){
    size_t x = 0;
    size_t y = 0;
    long long trees = 0;

    while(y < height){
      if(map[y].at(x) == '#')
        trees++;
      
      x = (x + right[i]) % width;
      y += down[i];
    }

    cout << i << ": " << trees << " ";
    result *= trees;

  }

  cout << "answer: " << result << endl;

  return 0;
}