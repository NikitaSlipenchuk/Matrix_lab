#include <iostream>
using namespace std;

template <typename T>
struct Node {
  T data;
  Node* pNext;
  Node* pPrevious;
  Node(T data = T(), Node* pNext = nullptr, Node* pPrevious = nullptr) : data(data), pNext(pNext), pPrevious(pPrevious) {};
};

template <typename T>
class LinkedList {
private:
  Node<T> *head;
public:
  LinkedList() {
    head = nullptr;
  }

  void push_head(T data) {
    if (head == nullptr) {
      head = new Node<T>(data);
    }
    else {
      head->pPrevious = new Node<T>(data, head, nullptr);
      head = head->pPrevious;
    }
  }

  void  push_tail(T data) {
    if (head == nullptr) {
      head = new Node<T>(data);
    }
    else {
      Node<T> *current = head;
      while (current->pNext != nullptr) {
        current = current->pNext;
      }
      current->pNext = new Node<T>(data,nullptr,current);
    }
  }

  T operator[](size_t index) {
    Node<T>* current = head;
    size_t cnt = 0;
    while (head->pNext != nullptr) {
      if (cnt == index) {
        return current->data;
      }
      cnt++;
      current = current->pNext;
    }
    return NULL;
  }

  T& operator[](size_t index) {
    Node<T>* current = head;
    size_t cnt = 0;
    while (head->pNext != nullptr) {
      if (cnt == index) {
        return current->data;
      }
      cnt++;
      current = current->pNext;
    }
    return NULL;
  }

  ~LinkedList(){
    delete [] head;
  }
};

int main() {
  cout << "hello world";
  LinkedList<int>list;
  list.push_head(12);
  list.push_tail(24);
  list.push_head(222);
  cout << list[1];
}
