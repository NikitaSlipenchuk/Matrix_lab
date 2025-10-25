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

  LinkedList(size_t size) {
    throw logic_error("Only natural numbers");
    head = new Node<T>(1);
    Node<T>* current = head;
    for (int i = 2; i <= size; i++) {
      current->pNext = new Node<T>(i);
      current = current->pNext;
    }
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
    }
  }

  void pop_tail() {
    if (head == nullptr) {
      throw logic_error("There is not elements in list");
    }
    Node<T>* current = head;
    while (current->pNext != nullptr) {
      current = current->pNext;
    }
    Node<T>* buf = current;
    current = current->pPrevious;
    current->pPrevious = nullptr;
  }

  void delete_node(const T& data) {
    Node<T>* current = head;
    while (current != nullptr) {
      if (current->data == data) {
        Node<T>* nodeToDelete = current;
        if (current->pPrevious != nullptr) {
          current->pPrevious->pNext = current->pNext;
        }
        else {
          head = current->pNext;
        }
        if (current->pNext != nullptr) {
          current->pNext->pPrevious = current->pPrevious;
        }
        current = current->pNext;
        delete nodeToDelete;
      }
      else {
        current = current->pNext;
      }
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
  
  LinkedList<int>list(60);
  for (int i = 0; i < 60; i++) {
    cout << list[i] << endl;
  }

}
