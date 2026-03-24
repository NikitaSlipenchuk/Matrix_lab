#include <iostream>

using namespace std;

struct Node {
  int value;
  Node* left = nullptr;
  Node* right = nullptr;
  Node() = default;
  Node(int value) : value(value), left(nullptr), right(nullptr) {};
  Node(int value, Node* left, Node* right) : value(value), left(left), right(right) {};
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

  void print(Node* node) const {
    if (node == nullptr) {
      return;
    }
    print(node->left);
    cout << node->value;
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

  void delete_tree(Node* node)
  {
    if (!node)
      return;

    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
  }

  Node* copy_tree(Node* other)
  {
    if (!other)
      return nullptr;
    Node* res = new Node(other->value);
    res->left = copy_tree(other->left);
    res->right = copy_tree(other->right);
    return res;
  }

public:
  set() = default;

  set(const set& other) {
    _root = new Node();
    copy_tree(other._root);
  }

  set& operator=(const set& other)
  {
    if (this != &other)
    {
      set tmp(other);
      std::swap(tmp._root, this->_root);
      return *this;
    }
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

  void print() const{
    print(_root);
    cout << endl;
  }

  bool erase(int value)
  {
    if (!contains(_root, value))
      return false;

    _root = erase(_root, value);
    return true;
  }

  ~set() {
    delete_tree(_root);
  }
 
};

int main() {
  set s;
    
    // Вставка
    s.insert(5);
    s.insert(3);
    s.insert(7);
    s.insert(2);
    s.insert(4);
    s.insert(6);
    s.insert(8);
    
    cout << "Initial: ";
    s.print();  // 2345678
    
    // Удаление листа
    s.erase(2);
    cout << "After erase(2): ";
    s.print();  // 345678
    
    // Удаление узла с одним ребенком
    s.erase(3);
    cout << "After erase(3): ";
    s.print();  // 45678
    
    // Удаление узла с двумя детьми
    s.erase(5);
    cout << "After erase(5): ";
    s.print();  // 4678
    
    // Попытка удалить несуществующий элемент
    if (!s.erase(100)) {
        cout << "100 not found" << endl;
    }
    
    return 0;
}
