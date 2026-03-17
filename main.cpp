#include <iostream>

using namespace std;

struct Node {
  int value;
  Node* left = nullptr;
  Node* right = nullptr;
  Node() = default;
  Node(int value) : value(value), left(nullptr), right(nullptr) {};
  Node(int value, Node* left, Node* right) : value(value), left(left), right(right) {};
  ~Node() {
    if (left) {
      delete left;
    }
    if (right) {
      delete right;
    }
  }
};

class set {
private:
  Node* _root = nullptr;

  Node* insert(Node* node, int value) {
    if (node == nullptr) {
      return new Node(value);
    }
    if (value < node->value) {
      node->left = insert(node->left, value);
    }
    else if (value > node->value) {
      node->right = insert(node->right, value);
    }
    return node;
  }

  bool contains(Node* node, int value) const {
    if (node == nullptr) {
      return false;
    }
    if (value < node->value) {
      return contains(node->left, value);
    }
    else if (value > node->value) {
      return contains(node->right, value);
    }
    else { return true; }
  }

public:
  set() = default;

  bool contains(int key) const {
    return contains(_root, key);
  }

  bool insert(int value) {
    if (contains(_root, value)) {
      return false;
    }
    _root = insert(_root, value);
    return true;
  }
};

int main() {
  
}
