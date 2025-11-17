#include <iostream>
#include <vector>
#include <random>
#include "LinkedList.h"
using namespace std;

struct stats {
  size_t comparison_count = 0;
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
      st.comparison_count++;
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

stats comb_sort(vector<int>&vec) {
  stats st;
  int n = vec.size();
  int step = n;
  bool swapped = true;
  while (step > 1 || swapped) {
    step = (step * 10) / 13;
    if (step < 1) {
      step = 1;
    }
    swapped = false;
    for (int i = 0; i + step < n; i++) {
      if (vec[i] > vec[i + step]) {
        swap(vec[i], vec[i + step]);
        swapped = true;
        st.copy_count += 3;
      }
      st.comparison_count += 1;
    }
  }
  return st;
}


void distribute(const vector<int>& input, vector<int>& start_to_end, vector<int>& end_to_start, stats& st) {
  size_t i = 0, j = input.size() - 1;
  bool write_to_start_to_end = true;
  while (i <= j) {
    if (write_to_start_to_end) {
      start_to_end.push_back(input[i++]);
      st.copy_count++;
      while (i <= j && i < input.size()) {
        st.comparison_count++;
        if (input[i] >= start_to_end.back()) {
          start_to_end.push_back(input[i++]);
          st.copy_count++;
        }
        else {
          break;
        }
      }
    }
    else {  
      end_to_start.push_back(input[j--]);
      st.copy_count++;
      while (i <= j && j >= 0) {
        st.comparison_count++;
        if (input[j] <= end_to_start.back()) {
          end_to_start.push_back(input[j--]);
          st.copy_count++;
        }
        else {
          break;
        }
      }
      reverse(end_to_start.begin() + (end_to_start.size() - (input.size() - 1 - j)), end_to_start.end());
    }
    write_to_start_to_end = !write_to_start_to_end;
  }
}


void merge(const vector<int>& start_to_end, const vector<int>& end_to_start, vector<int>& output, stats& st) {
  size_t i = 0, j = 0;
  while (i < start_to_end.size() && j < end_to_start.size()) {
    size_t start_left = i;
    while (i + 1 < start_to_end.size() && start_to_end[i] <= start_to_end[i + 1]) {
      st.comparison_count++;
      i++;
    }
    size_t end_left = i;
    size_t start_right = j;
    while (j + 1 < end_to_start.size() && end_to_start[j] <= end_to_start[j + 1]) {
      st.comparison_count++;
      j++;
    }
    size_t end_right = j;
    i = start_left;
    j = start_right;
    while (i <= end_left && j <= end_right) {
      st.comparison_count++;
      if (start_to_end[i] <= end_to_start[j]) {
        output.push_back(start_to_end[i++]);
      }
      else {
        output.push_back(end_to_start[j++]);
      }
      st.copy_count++;
    }
    while (i <= end_left) {
      output.push_back(start_to_end[i++]);
      st.copy_count++;
    }
    while (j <= end_right) {
      output.push_back(end_to_start[j++]);
      st.copy_count++;
    }
    i = end_left + 1;
    j = end_right + 1;
  }
  while (i < start_to_end.size()) {
    output.push_back(start_to_end[i++]);
    st.copy_count++;
  }
  while (j < end_to_start.size()) {
    output.push_back(end_to_start[j++]);
    st.copy_count++;
  }
}

stats sort_natural_two_way_merge(vector<int>& arr) {
  stats st;
  vector<int>current = arr;
  while (true) {
    vector<int> start_to_end, end_to_start;
    distribute(current, start_to_end, end_to_start, st);
    if (end_to_start.empty()) {
      arr = start_to_end;
      break;
    }
    vector<int> merged;
    merge(start_to_end, end_to_start, merged, st);
    bool sorted = true;
    for (size_t i = 1; i < merged.size(); i++) {
      st.comparison_count++;
      if (merged[i] < merged[i - 1]) {
        sorted = false;
        break;
      }
    }
    current = merged;
    if (sorted) {
      arr = current;
      break;
    }
  }
  return st;
}



int main() {
  vector<int>vec(30);
  for (int i = 0; i < 100; i++) {
    random_device engine;
    uniform_int_distribution distribution(1, 400);
    for (int i = 0; i < 30; i++)
    {
      vec[i] = distribution(engine);
    }
    cout <<endl << "mas= ";
    for (int i = 0; i < 30; i++) {
      cout << vec[i] << " ";
    }
    stats st;
    st = sort_natural_two_way_merge(vec);
    cout << endl << "mas= ";
    for (int i = 0; i < 30; i++) {
      cout << vec[i] << " ";
    }
    cout << endl << st.comparison_count << endl;
    cout << endl << st.copy_count;
  }
}
