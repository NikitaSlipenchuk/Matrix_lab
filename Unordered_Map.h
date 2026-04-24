#ifndef UNORDERED_MAP
#define UNORDERED_MAP

#include <iostream>
#include <exception>
#include <cmath>


template <class T>
struct Node {
  int key = 0;
  T value = T(0);
  Node<T>* next = nullptr;
  Node() = default;
  Node(int key, T value) : key(key), value(value), next(nullptr) {}
};

template <class T>
class UnorderedMap
{
  Node<T>** _table = nullptr;
  size_t _capacity = 0;

  size_t next_power_of_two(size_t n) const {
    if (n == 0) return 1;
    size_t power = 1;
    while (power < n) {
      power <<= 1;
    }
    return power;
  }

  size_t hash(int key) const {
    const size_t w = 32;
    const size_t a = 1664525;
    size_t r = static_cast<size_t>(log2(_capacity));
    return (static_cast<size_t>(abs(key)) * a) >> (w - r);
  }

public:
  UnorderedMap(size_t capacity)
  {
    _capacity = next_power_of_two(capacity);
    _table = new Node<T>*[_capacity];
    for (size_t i = 0; i < _capacity; i++)
      _table[i] = nullptr;
  }

  UnorderedMap(const UnorderedMap<T>& other) : _capacity(other._capacity)
  {
    _table = new Node<T>*[_capacity];
    for (size_t i = 0; i < _capacity; i++) {
      if (other._table[i] == nullptr) {
        _table[i] = nullptr;
        continue;
      }
      _table[i] = new Node<T>*(other._table[i]->key, other._table[i]->value);
      Node<T>* iter = _table[i];
      Node<T>* other_iter = other._table[i]->next;
      while (other_iter)
      {
        iter->next = new Node(other_iter->key, other_iter->value);
        other_iter = other_iter->next;
        iter = iter->next;
      }
    }
  }

  UnorderedMap<T>& operator=(const UnorderedMap<T>& other)
  {
    if (this != &other)
    {
      UnorderedMap<T> tmp(other);
      std::swap(_table, tmp._table);
      std::swap(_capacity, tmp._capacity);
      return *this;
    }
  }


  void print() const
  {
    for (size_t i = 0; i < _capacity; i++)
    {
      Node<T>* iter = _table[i];
      while (iter)
      {
        std::cout << "{" << iter->key << " : " << iter->value << "} "<<"\n";
        iter = iter->next;
      }
    }
    std::cout << "\n";
  }

  bool insert(int key, const T& value)
  {
    Node<T>* iter = _table[hash(key)];
    if (!iter)
    {
      _table[hash(key)] = new Node<T>(key, value);
      return true;
    }

    while (iter)
    {
      if (iter->key == key)
      {
        return false;
      }
      if (iter->next == nullptr)
        break;
      iter = iter->next;
    }
    iter->next = new Node<T>(key, value);
    return true;
  }

  void insert_or_assign(int key, const T& value)
  {
    Node<T>* iter = _table[hash(key)];
    if (!iter)
    {
      _table[hash(key)] = new Node<T>(key, value);
      return;
    }

    while (iter)
    {
      if (iter->key == key)
      {
        iter->value = value;
        return;
      }
      if (iter->next == nullptr)
        break;
      iter = iter->next;
    }
    iter->next = new Node<T>(key, value);
  }

  bool contains(T& value) const
  {
    for (size_t i = 0; i < _capacity; i++)
    {
      Node<T>* iter = _table[i];
      while (iter)
      {
        if (iter->value == value)
        {
          return true;
        }
        iter = iter->next;
      }
    }
    return false;
  }


  Node<T>* search(int key)
  {
    Node<T>* iter = _table[hash(key)];
    while (iter)
    {
      if (iter->key == key)
      {
        return iter;
      }
      iter = iter->next;
    }
    return nullptr;
  }

  bool erase(int key)
  {
    size_t index = hash(key);
    Node<T>* current = _table[index];
    Node<T>* previous = nullptr;
    while (current != nullptr)
    {
      if (current->key == key)
      {
        if (previous == nullptr)
        {
          _table[index] = current->next;
        }
        else
        {
          previous->next = current->next;
        }
        delete current;
        return true;  
      }
      previous = current;
      current = current->next;
    }
    return false;
  }


  int count(int key) const
  {
    Node<T>* iter = _table[hash(key)];
    int result = 0;
    while (iter)
    {
      result++;
      iter = iter->next;
    }
    return result;
  }

  ~UnorderedMap()
  {
    for (size_t i = 0; i < _capacity; i++)
    {
      Node<T>* head = _table[i];
      while (head)
      {
        Node<T>* next = head->next;
        delete head;
        head = next;
      }
    }
    delete[] _table;
  }

};


#endif
