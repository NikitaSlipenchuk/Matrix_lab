#include <iostream>
#include "Unordered_Map.h"
#include <vector>
#include <random>
using namespace std;

void example(vector<int>& vec) {
  int count = 0;
  UnorderedMap<int>map(vec.size());
  for (int i = 0; i < vec.size(); i++) {
    bool flag = map.insert(vec[i], 1);
    if (flag == false) {
      int new_vaue = map.search(vec[i])->value;
      map.insert_or_assign(vec[i], new_vaue + 1);
    }
  }
  map.print();
}

int main() {
  vector<int>vec;
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(-100, 100);  
  for (int i = 0; i < 20; i++) {
    vec.push_back(dist(gen));  
    std::cout << vec[i] << " ";
  }
  example(vec);
}
