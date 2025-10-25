#ifndef FUNCTION
#define FUNCTION
#include<iostream>
#include "LinkedList.h"
using namespace std;

template<typename T>
void lucky_numbers(LinkedList<T>& list, size_t size) {
  int rm_index = 2, buffer = 0;
  for (int i = 0; i < size; i++) {
    if ((i + 1) % rm_index == 0) {
      list[i] = 0;
      buffer++;
    }
  }
  size = size - buffer;
  buffer = 0;
  list.delete_node(0);
  Node<T>* buf = list.get_head();
  buf = buf->pNext;
  rm_index = buf->data;
  while (rm_index < size) {
    for (int i = 0; i < size; i++) {
      if ((i + 1) % rm_index == 0) {
        list[i] = 0;
        buffer++;
      }
    }
    size = size - buffer;
    buffer = 0;
    list.delete_node(0);
    buf = buf->pNext;
    rm_index = buf->data;
  }
}

void example() {
  LinkedList<int>list1;
  list1.push_head(3);
  list1.push_tail(4);
  list1.push_head(5);
  cout << "list1=";
  for (int i = 0; i < 3; i++) {
    cout << list1[i] << " ";
  }
  LinkedList<int>list2;
  list2.push_head(14);
  list2.push_head(22);
  list2.push_head(list1);
  cout << endl << "list2+list1=";
  for (int i = 0; i < 5; i++) {
    cout << list2[i] << " ";
  }
  list2.pop_head();
  cout << endl << "list2(pop_head)=";
  for (int i = 0; i < 4; i++) {
    cout << list2[i] << " ";
  }
  list1.delete_node(3);
  cout << endl << "list1(remove 3)=";
  for (int i = 0; i < 2; i++) {
    cout << list1[i] << " ";
  }
  cout << endl << "My task:";
  LinkedList<int>list(25);
  cout << endl << "started mas=";
  for (int i = 0; i < 25; i++) {
    cout << list[i] << " ";
  }
  LinkedList<int>started_list(25);
  lucky_numbers(list, 25);
  cout << endl << "Anlucky numbers:";
  for (int i = 0; i < 25; i++) {
    if (!list.is_there(started_list[i])) {
      cout << started_list[i] << " ";
    }
  }
}
#endif
