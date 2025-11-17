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



int main() {
  LinkedList<int>list(20);
  for (int i = 0; i < 20; i++) {
    list[i] = rand() % 100;
  }
  for (int i = 0; i < 20; i++) {
    cout << list[i] << " ";
  }
  cout << endl << "Stat  inf:" << endl;
  stats st;
  st = select_sort(list);
  for (int i = 0; i < 20; i++) {
    cout << list[i] << " ";
  }
  cout << endl << st.comporation_count << endl;
  cout << endl << st.copy_count << endl;
}
