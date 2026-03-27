#ifndef SET_H
#define SET_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
  int value;
  Node* left = nullptr;
  Node* right = nullptr;
  int height = 0;
  Node() = default;
  Node(int value) : value(value), left(nullptr), right(nullptr) {};
  Node(int value, Node* left, Node* right) : value(value), left(left), right(right) {};
};

class set {
private:
  Node* _root = nullptr;

  int get_height(Node* node) const {
    return node == nullptr ? -1 : node->height;
  }

  void update_hight(Node* node) {
    node->height = max(get_height(node->left), get_height(node->right)) + 1;
  }

  int get_balanced(Node* node) const {
    return (!node) ? 0 : get_height(node->right) - get_height(node->left);
  }

  void rotate_right(Node* node) {
    swap(node->value, node->left->value);
    Node* buf = node->right;
    node->right = node->left;
    node->left = node->right->left;
    node->right->left = node->right->right;
    node->right->right = buf;
    update_hight(node->right);
    update_hight(node);
  }

  void rotate_left(Node* node) {
    swap(node->value, node->right->value);
    Node* buf = node->left;
    node->left = node->right;
    node->right = node->right->right;
    node->left->right = node->left->left;
    node->left->left = buf;
    update_hight(node->left);
    update_hight(node);
  }

  void balance(Node* node) {
    int balance = get_balanced(node);
    if (balance == -2) {
      if (get_balanced(node->left) == 1) {
        rotate_left(node->left);
      }
      rotate_right(node);
    }
    else if (balance == 2) {
      if (get_balanced(node->right) == -1) {
        rotate_right(node->right);
      }
      rotate_left(node);
    }
  }

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
    update_hight(node);
    balance(node);
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
    else {
      return true;
    }
  }

  void print(Node* node) const {
    if (node == nullptr) {
      return;
    }
    print(node->left);
    cout << node->value << " ";
    print(node->right);
  }

  Node* erase(Node* root, int value) {
    if (!root)
      return nullptr;

    if (value < root->value) {
      root->left = erase(root->left, value);
    }
    else if (value > root->value) {
      root->right = erase(root->right, value);
    }
    else {
      Node* oldNode = root;

      if (!root->left || !root->right) {
        root = (!root->left) ? root->right : root->left;
        delete oldNode;
      }
      else {
        Node* max_in_left = get_max(root->left);
        root->value = max_in_left->value;
        root->left = erase(root->left, max_in_left->value);
      }
    }
    if (root != nullptr) {
      update_hight(root);
      balance(root);
    }
    return root;
  }

  Node* get_max(Node* root) {
    if (!root) {
      return nullptr;
    }
    if (!root->right) {
      return root;
    }
    return get_max(root->right);
  }

  Node* get_min(Node* root) {
    if (!root) {
      return nullptr;
    }
    if (!root->left) {
      return root;
    }
    return get_min(root->left);
  }

  void delete_tree(Node* node) {
    if (!node)
      return;

    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
  }

  Node* copy_tree(Node* other) {
    if (!other)
      return nullptr;
    Node* res = new Node(other->value);
    res->left = copy_tree(other->left);
    res->right = copy_tree(other->right);
    return res;
  }

  void to_vector(Node* node, std::vector<int>& result) const {
    if (node == nullptr) {
      return;
    }
    to_vector(node->left, result);
    result.push_back(node->value);
    to_vector(node->right, result);
  }

public:
  set() = default;

  set(const set& other) {
    _root = copy_tree(other._root);
  }

  set& operator=(const set& other) {
    if (this != &other) {
      set tmp(other);
      std::swap(tmp._root, this->_root);
    }
    return *this;
  }

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

  void print() const {
    print(_root);
    cout << endl;
  }

  bool erase(int value) {
    if (!contains(_root, value))
      return false;

    _root = erase(_root, value);
    return true;
  }

  bool strictly_balanced() const {
    return abs(get_balanced(_root)) <= 1 ? true : false;
  }

  std::vector<int> to_vector() const {
    std::vector<int> result;
    to_vector(_root, result);
    return result;
  }

  ~set() {
    delete_tree(_root);
  }
};

#endif
