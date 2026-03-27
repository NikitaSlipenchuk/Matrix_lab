#ifndef STATISTICS_H
#define STATISTICS_H

#include "set.h"
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

size_t lcg() {
  static size_t x = 0;
  x = (1021 * x + 24631) % 116640;
  return x;
}

struct Statistics {
  double insert_time;
  double search_time;
  double add_remove_time;
};

Statistics test_avl(int size, int insert_attempts, int search_attempts, int add_remove_attempts) {
  Statistics stats = { 0, 0, 0 };

  double total_insert_time = 0;
  for (int attempt = 0; attempt < insert_attempts; attempt++) {
    set avl;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < size; i++) {
      avl.insert(lcg() % (size * 10) + 1);
    }

    auto end = high_resolution_clock::now();
    total_insert_time += duration_cast<microseconds>(end - start).count();
  }
  stats.insert_time = total_insert_time / insert_attempts / 1000000.0;

  set avl;
  for (int i = 0; i < size; i++) {
    avl.insert(lcg() % (size * 10) + 1);
  }

  vector<int> search_values(search_attempts);
  for (int i = 0; i < search_attempts; i++) {
    search_values[i] = lcg() % (size * 2);
  }

  auto start = high_resolution_clock::now();
  for (int i = 0; i < search_attempts; i++) {
    avl.contains(search_values[i]);
  }
  auto end = high_resolution_clock::now();
  stats.search_time = duration_cast<microseconds>(end - start).count() / (double)search_attempts;

  vector<int> add_values(add_remove_attempts);
  vector<int> remove_values(add_remove_attempts);
  for (int i = 0; i < add_remove_attempts; i++) {
    add_values[i] = lcg() % (size * 3) + size;
    remove_values[i] = lcg() % (size * 2);
  }

  start = high_resolution_clock::now();
  for (int i = 0; i < add_remove_attempts; i++) {
    avl.insert(add_values[i]);
    avl.erase(remove_values[i]);
  }
  end = high_resolution_clock::now();
  stats.add_remove_time = duration_cast<microseconds>(end - start).count() / (double)add_remove_attempts;

  return stats;
}

Statistics test_vector(int size, int insert_attempts, int search_attempts, int add_remove_attempts) {
  Statistics stats = { 0, 0, 0 };

  double total_insert_time = 0;
  for (int attempt = 0; attempt < insert_attempts; attempt++) {
    vector<int> vec;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < size; i++) {
      vec.push_back(lcg() % (size * 10) + 1);
    }

    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());

    auto end = high_resolution_clock::now();
    total_insert_time += duration_cast<microseconds>(end - start).count();
  }
  stats.insert_time = total_insert_time / insert_attempts / 1000000.0;

  vector<int> vec;
  for (int i = 0; i < size; i++) {
    vec.push_back(lcg() % (size * 10) + 1);
  }
  sort(vec.begin(), vec.end());
  vec.erase(unique(vec.begin(), vec.end()), vec.end());

  vector<int> search_values(search_attempts);
  for (int i = 0; i < search_attempts; i++) {
    search_values[i] = lcg() % (size * 2);
  }

  auto start = high_resolution_clock::now();
  for (int i = 0; i < search_attempts; i++) {
    binary_search(vec.begin(), vec.end(), search_values[i]);
  }
  auto end = high_resolution_clock::now();
  stats.search_time = duration_cast<microseconds>(end - start).count() / (double)search_attempts;

  vector<int> add_values(add_remove_attempts);
  vector<int> remove_values(add_remove_attempts);
  for (int i = 0; i < add_remove_attempts; i++) {
    add_values[i] = lcg() % (size * 3) + size;
    remove_values[i] = lcg() % (size * 2);
  }

  start = high_resolution_clock::now();
  for (int i = 0; i < add_remove_attempts; i++) {
    auto it = lower_bound(vec.begin(), vec.end(), add_values[i]);
    if (it == vec.end() || *it != add_values[i]) {
      vec.insert(it, add_values[i]);
    }

    it = lower_bound(vec.begin(), vec.end(), remove_values[i]);
    if (it != vec.end() && *it == remove_values[i]) {
      vec.erase(it);
    }
  }
  end = high_resolution_clock::now();
  stats.add_remove_time = duration_cast<microseconds>(end - start).count() / (double)add_remove_attempts;

  return stats;
}

void print_table(const vector<int>& sizes) {
  int insert_attempts = 100;
  int search_attempts = 1000;
  int add_remove_attempts = 1000;

  cout << fixed << setprecision(6);
  cout << "\n" << string(100, '=') << endl;
  cout << "PERFORMANCE COMPARISON: AVL TREE vs STD::VECTOR" << endl;
  cout << string(100, '=') << endl;

  for (int size : sizes) {
    cout << "\n--- SIZE: " << size << " elements ---\n" << endl;

    cout << "Testing AVL Tree..." << endl;
    Statistics avl_stats = test_avl(size, insert_attempts, search_attempts, add_remove_attempts);

    cout << "Testing std::vector..." << endl;
    Statistics vec_stats = test_vector(size, insert_attempts, search_attempts, add_remove_attempts);

    cout << "\n";
    cout << left << setw(20) << "Operation"
      << right << setw(20) << "AVL Tree"
      << right << setw(20) << "std::vector"
      << right << setw(20) << "AVL/Vector Ratio" << endl;
    cout << string(80, '-') << endl;

    cout << left << setw(20) << "Insertion (sec)"
      << right << setw(20) << avl_stats.insert_time
      << right << setw(20) << vec_stats.insert_time;
    if (vec_stats.insert_time > 0)
      cout << right << setw(20) << (avl_stats.insert_time / vec_stats.insert_time);
    else
      cout << right << setw(20) << "N/A";
    cout << endl;

    cout << left << setw(20) << "Search (mcs/op)"
      << right << setw(20) << avl_stats.search_time
      << right << setw(20) << vec_stats.search_time;
    if (vec_stats.search_time > 0)
      cout << right << setw(20) << (avl_stats.search_time / vec_stats.search_time);
    else
      cout << right << setw(20) << "N/A";
    cout << endl;

    cout << left << setw(20) << "Add+Remove (mcs/op)"
      << right << setw(20) << avl_stats.add_remove_time
      << right << setw(20) << vec_stats.add_remove_time;
    if (vec_stats.add_remove_time > 0)
      cout << right << setw(20) << (avl_stats.add_remove_time / vec_stats.add_remove_time);
    else
      cout << right << setw(20) << "N/A";
    cout << endl;
  }

  cout << "\n" << string(100, '=') << endl;
  cout << "Note: Ratio < 1 means AVL Tree is faster" << endl;
  cout << string(100, '=') << endl;
}

vector<int> search_repeat(vector<int>& vec) {
  set s, ans;
  s.insert(vec[0]);
  for (int i = 1; i < vec.size(); i++) {
    if (s.contains(vec[i]) == true) {
      ans.insert(vec[i]);
    }
    else {
      s.insert(vec[i]);
    }
  }
  vector<int> answer = ans.to_vector();
  return answer;
}

#endif
