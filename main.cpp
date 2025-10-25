#include <iostream>
#include <stdexcept>
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
  Node<T>* head;
public:
  LinkedList() {
    head = nullptr;
  }

  LinkedList(const LinkedList &list) {
    if (list.head == nullptr) {
      head = nullptr;
    }
    else {
      Node<T>* current = list.head;
      head = new Node<T>(list.head->data);
      current = current->pNext;
      Node<T>* currentthis = head;
      while (current != nullptr) {
        currentthis->pNext = new Node<T>(current->data, nullptr, currentthis);
        currentthis = currentthis->pNext;
        current = current->pNext;
      }
    }
  }

  LinkedList& operator=(const LinkedList &list) {
    Node<T>* current = list.head;
    head = new Node<T>(list.head->data);
    current = current->pNext;
    Node<T>* currentthis = head;
    while (current != nullptr) {
      currentthis->pNext = new Node<T>(current->data, nullptr, currentthis);
      currentthis = currentthis->pNext;
      current = current->pNext;
    }
    return *this;
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

  void push_head(const LinkedList &lhs) {
    if (lhs.head == nullptr) return;  // защита от пустого списка
    LinkedList<T>buffer(lhs);
    if (head == nullptr) {
      head = buffer.head;
      buffer.head = nullptr;  // предотвращаем удаление в деструкторе buffer
      return;
    }
    Node<T>* current = buffer.head;
    while (current->pNext != nullptr) {
      current = current->pNext;
    }
    current->pNext = head;
    head->pPrevious = current;
    head = buffer.head;
    buffer.head = nullptr;
  }

  void  push_tail(T data) {
    if (head == nullptr) {
      head = new Node<T>(data);
    }
    else {
      Node<T>* current = head;
      while (current->pNext != nullptr) {
        current = current->pNext;
      }
      current->pNext = new Node<T>(data, nullptr, current);
    }
  }

  void push_tail(const LinkedList& rhs) {
    if (rhs.head == nullptr) return;
    LinkedList<T>buffer(rhs);
    if (head == nullptr) {
      head = buffer.head;
      buffer.head = nullptr;
      return;
    }
    Node<T>* current = head;
    while (current->pNext != nullptr) {
      current = current->pNext;
    }
    current->pNext = buffer.head;
    buffer.head->pPrevious = current;
    buffer.head = nullptr;
  }

  T operator[](size_t index) const {
    Node<T>* current = head;
    size_t cnt = 0;
    while (current != nullptr) {
      if (cnt == index) {
        return current->data;
      }
      cnt++;
      current = current->pNext;
    }
    throw out_of_range("Index out of range");
  }

  T& operator[](size_t index) {
    Node<T>* current = head;
    size_t cnt = 0;
    while (current!= nullptr) {
      if (cnt == index) {
        return current->data;
      }
      cnt++;
      current = current->pNext;
    }
    throw out_of_range("Index out of range");
  }

  void pop_head() {
    if (head == nullptr) {
      throw logic_error("There is not elements in list");
    }
    if (head->pNext == nullptr) {
      Node<T>* buf = head;
      head = nullptr;
      delete buf;
    }
    else {
      Node<T>* buf = head;
      head = this->head->pNext;
      head->pPrevious = nullptr;
      delete buf;
    }
  }

  ~LinkedList(){
    Node<T>* current = head;
    while (current != nullptr) {
      Node<T>* next = current->pNext;
      delete current;
      current = next;
    }
  }
};

int main() {
  cout << "hello world";
  LinkedList<int>list1;
  list1.push_head(12);
  LinkedList<int>list2(list1);
  list2.pop_head();
}
