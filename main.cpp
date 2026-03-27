#include<iostream>
#include "set.h"
#include "func.h"

int main() {
  vector<int> sizes = { 1000, 10000, 100000 };
  print_table(sizes);

  vector<int> vec = { 3, 2, 2, 4, 2 };
  vector<int> answer = search_repeat(vec);

  for (int i = 0; i < answer.size(); i++) {
    cout << answer[i] << " ";
  }

  return 0;
}
