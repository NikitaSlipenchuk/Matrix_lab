#include <iostream>
#include <vector>
#include "LinkedList.h"
using namespace std;

struct stats {
  size_t comporation_count = 0;
  size_t copy_count = 0;
};

stats select_sort(LinkedList<int>& list) {
  struct stats st;
  Node<int>* cur = list.get_head();
  while (cur != nullptr) {
    Node<int>* minNode = cur;
    Node<int>* tmp = cur->pNext;
    while (tmp != nullptr) {
      if (tmp->data < minNode->data) {
        minNode = tmp;
      }
      st.comporation_count++;
      tmp = tmp->pNext;
    }
    if (cur != minNode) {
      swap(cur->data, minNode->data); //можно конечно менять не значения,а сами узлы, но тогда действий будет больше
      st.copy_count += 3;
    }
    cur = cur->pNext;
  }
  return st;
}

stats comb_sort(vector<int>&vec) {
  stats st;
  int n = vec.size();
  int step = n;
  bool swapped = true;
  while (step > 1 || swapped) {
    step = (step * 10) / 13;
    if (step < 1) {
      step = 1;
    }
    swapped = false;
    for (int i = 0; i + step < n; i++) {
      if (vec[i] > vec[i + step]) {
        swap(vec[i], vec[i + step]);
        swapped = true;
        st.copy_count += 3;
      }
      st.comporation_count += 1;
    }
  }
  return st;
}



int main() {
  vector<int>vec(20);
  for (int i = 0; i < 20; i++) {
    vec[i] = rand() % 150;
  }
  cout << "mas= ";
  for (int i = 0; i < 20; i++) {
    cout << vec[i] << " ";
  }
  stats st;
  st = comb_sort(vec);
  cout <<endl<< "mas= ";
  for (int i = 0; i < 20; i++) {
    cout << vec[i] << " ";
  }
  cout <<endl<< st.comporation_count<<endl;
  cout <<endl<<st.copy_count;

}
