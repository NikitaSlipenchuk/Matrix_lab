#include <iostream>
#include <vector>
#include <random>
#include "LinkedList.h"
#include <chrono> 
using namespace std;

struct stats {
  size_t comparison_count = 0;
  size_t copy_count = 0;

  stats& operator+=(const stats& other) {
    comparison_count += other.comparison_count;
    copy_count += other.copy_count;
    return *this;
  }
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
      swap(cur->data, minNode->data); //ìîæíî êîíå÷íî ìåíÿòü íå çíà÷åíèÿ,à ñàìè óçëû, íî òîãäà äåéñòâèé áóäåò áîëüøå
      st.copy_count += 3;
    }
    cur = cur->pNext;
  }
  return st;
}

stats comb_sort(vector<int>& vec) {
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


void test_random_arrays_task3() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  cout << "=== ÑËÓ×ÀÉÍÛÅ ÌÀÑÑÈÂÛ (ñðåäíåå çà 100 çàïóñêîâ) ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    stats total_stats;
    for (int i = 0; i < 100; i++) {
      vector<int> vec(size);
      mt19937 generator(seed + i);
      uniform_int_distribution<int> distribution(1, size * 10);

      for (int j = 0; j < size; j++) {
        vec[j] = distribution(generator);
      }
      vector<int> vec_copy = vec;
      stats current_stats = sort_natural_two_way_merge(vec_copy);
      total_stats += current_stats;
    }
    double avg_comparisons = static_cast<double>(total_stats.comparison_count) / 100;
    double avg_copies = static_cast<double>(total_stats.copy_count) / 100;
    cout << size << "\t" << avg_comparisons << "\t" << avg_copies << endl;
  }
}

void test_sorted_arrays_task3() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  cout << "\n=== ÎÒÑÎÐÒÈÐÎÂÀÍÍÛÅ ÌÀÑÑÈÂÛ ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    vector<int> vec(size);
    for (int i = 0; i < size; i++) {
      vec[i] = i;
    }
    stats st = sort_natural_two_way_merge(vec);
    cout << size << "\t" << st.comparison_count << "\t" << st.copy_count << endl;
  }
}

void test_reverse_sorted_arrays_task3() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  cout << "\n=== ÎÁÐÀÒÍÎ ÎÒÑÎÐÒÈÐÎÂÀÍÍÛÅ ÌÀÑÑÈÂÛ ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    vector<int> vec(size);
    for (int i = 0; i < size; i++) {
      vec[i] = size - i;
    }
    stats st = sort_natural_two_way_merge(vec);
    cout << size << "\t" << st.comparison_count << "\t" << st.copy_count << endl;
  }
}

void test_random_arrays_task2() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  cout << "=== ÑËÓ×ÀÉÍÛÅ ÌÀÑÑÈÂÛ (ñðåäíåå çà 100 çàïóñêîâ) ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    stats total_stats;
    for (int i = 0; i < 100; i++) {
      vector<int> vec(size);
      mt19937 generator(seed + i); 
      uniform_int_distribution<int> distribution(1, size * 10);
      for (int j = 0; j < size; j++) {
        vec[j] = distribution(generator);
      }
      vector<int> vec_copy = vec;
      stats current_stats = comb_sort(vec_copy);
      total_stats += current_stats;
    }
    double avg_comparisons = static_cast<double>(total_stats.comparison_count) / 100;
    double avg_copies = static_cast<double>(total_stats.copy_count) / 100;
    cout << size << "\t" << avg_comparisons << "\t" << avg_copies << endl;
  }
}

void test_sorted_arrays_task2() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  cout << "\n=== ÎÒÑÎÐÒÈÐÎÂÀÍÍÛÅ ÌÀÑÑÈÂÛ ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    vector<int> vec(size);
    for (int i = 0; i < size; i++) {
      vec[i] = i;
    }
    stats st = comb_sort(vec);
    cout << size << "\t" << st.comparison_count << "\t" << st.copy_count << endl;
  }
}

void test_reverse_sorted_arrays_task2() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  cout << "\n=== ÎÁÐÀÒÍÎ ÎÒÑÎÐÒÈÐÎÂÀÍÍÛÅ ÌÀÑÑÈÂÛ ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    vector<int> vec(size);
    for (int i = 0; i < size; i++) {
      vec[i] = size - i;
    }
    stats st = comb_sort(vec);
    cout << size << "\t" << st.comparison_count << "\t" << st.copy_count << endl;
  }
}

void test_random_arrays_task1() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  cout << "=== ÑËÓ×ÀÉÍÛÅ ÌÀÑÑÈÂÛ (ñðåäíåå çà 100 çàïóñêîâ) ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    stats total_stats;
    for (int i = 0; i < 100; i++) {
      LinkedList<int>list(size);
      mt19937 generator(seed + i); 
      uniform_int_distribution<int> distribution(1, size * 10);
      for (int j = 0; j < size; j++) {
        list[j] = distribution(generator);
      }
      LinkedList<int> list_copy = list;
      stats current_stats = select_sort(list_copy);
      total_stats += current_stats;
    }
    double avg_comparisons = static_cast<double>(total_stats.comparison_count) / 100;
    double avg_copies = static_cast<double>(total_stats.copy_count) / 100;
    cout << size << "\t" << avg_comparisons << "\t" << avg_copies << endl;
  }
}

void test_sorted_arrays_task1() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  cout << "\n=== ÎÒÑÎÐÒÈÐÎÂÀÍÍÛÅ ÌÀÑÑÈÂÛ ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    LinkedList<int> list(size);
    for (int i = 0; i < size; i++) {
      list[i] = i;
    }
    stats st = select_sort(list);
    cout << size << "\t" << st.comparison_count << "\t" << st.copy_count << endl;
  }
}

void test_reverse_sorted_arrays_task1() {
  vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };
  cout << "\n=== ÎÁÐÀÒÍÎ ÎÒÑÎÐÒÈÐÎÂÀÍÍÛÅ ÌÀÑÑÈÂÛ ===" << endl;
  cout << "Ðàçìåð\tÑðàâíåíèÿ\tÊîïèðîâàíèÿ" << endl;
  for (int size : sizes) {
    LinkedList<int> list(size);
    for (int i = 0; i < size; i++) {
      list[i] = size - i;
    }
    stats st = select_sort(list);
    cout << size << "\t" << st.comparison_count << "\t" << st.copy_count << endl;
  }
}

int main() {
  setlocale(LC_ALL, "ru");
  test_random_arrays_task1();
  test_sorted_arrays_task1();
  test_reverse_sorted_arrays_task1();
  test_random_arrays_task2();
  test_sorted_arrays_task2();
  test_reverse_sorted_arrays_task2();
  test_random_arrays_task3();
  test_sorted_arrays_task3();
  test_reverse_sorted_arrays_task3();
  return 0;
}
