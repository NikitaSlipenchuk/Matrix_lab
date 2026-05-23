#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <limits>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>  

template<typename Vertex, typename Distance = double>
class Graph {
private:
  struct EdgeInternal {
    size_t from_idx;
    size_t to_idx;
    Distance weight;
  };

  std::vector<Vertex> vertices_list;
  std::unordered_map<Vertex, size_t> vertex_to_index;
  std::vector<std::vector<std::optional<Distance>>> adjacency_matrix;

  size_t get_index(const Vertex& v) const {
    auto it = vertex_to_index.find(v);
    if (it == vertex_to_index.end()) {
      throw std::out_of_range("Vertex not found");
    }
    return it->second;
  }

public:
  struct Edge {
    Vertex from;
    Vertex to;
    Distance distance;

    bool operator==(const Edge& other) const {
      return from == other.from && to == other.to && distance == other.distance;
    }
  };

  Graph() = default;

  bool has_vertex(const Vertex& v) const {
    return vertex_to_index.find(v) != vertex_to_index.end();
  }

  bool add_vertex(const Vertex& v) {
    if (has_vertex(v)) return false;

    size_t new_idx = vertices_list.size();
    vertices_list.push_back(v);
    vertex_to_index[v] = new_idx;

    for (auto& row : adjacency_matrix) {
      row.resize(new_idx + 1);
    }
    adjacency_matrix.resize(new_idx + 1);
    adjacency_matrix.back().resize(new_idx + 1);

    return true;
  }

  bool remove_vertex(const Vertex& v) {
    if (!has_vertex(v)) return false;

    size_t idx_to_remove = vertex_to_index[v];
    vertex_to_index.erase(v);

    adjacency_matrix.erase(adjacency_matrix.begin() + idx_to_remove);
    for (auto& row : adjacency_matrix) {
      row.erase(row.begin() + idx_to_remove);
    }

    vertices_list.erase(vertices_list.begin() + idx_to_remove);
    for (size_t i = idx_to_remove; i < vertices_list.size(); ++i) {
      vertex_to_index[vertices_list[i]] = i;
    }

    return true;
  }

  std::vector<Vertex> vertices() const {
    return vertices_list;
  }

  void add_edge(const Vertex& from, const Vertex& to, const Distance& distance) {
    size_t from_idx = get_index(from);
    size_t to_idx = get_index(to);
    adjacency_matrix[from_idx][to_idx] = distance;
  }

  bool remove_edge(const Vertex& from, const Vertex& to) {
    size_t from_idx = get_index(from);
    size_t to_idx = get_index(to);
    if (adjacency_matrix[from_idx][to_idx].has_value()) {
      adjacency_matrix[from_idx][to_idx] = std::nullopt;
      return true;
    }
    return false;
  }

  bool remove_edge(const Edge& e) {
    size_t from_idx = get_index(e.from);
    size_t to_idx = get_index(e.to);
    if (adjacency_matrix[from_idx][to_idx].has_value() && e.distance == adjacency_matrix[from_idx][to_idx]) {
      adjacency_matrix[from_idx][to_idx] = std::nullopt;
      return true;
    }
    return false;
  }

  bool has_edge(const Vertex& from, const Vertex& to) const {
    if (!has_vertex(from) || !has_vertex(to)) return false;
    size_t from_idx = get_index(from);
    size_t to_idx = get_index(to);
    return adjacency_matrix[from_idx][to_idx].has_value();
  }

  bool has_edge(const Edge& e) const {
    if (!has_vertex(e.from) || !has_vertex(e.to)) return false;
    size_t from_idx = get_index(e.from);
    size_t to_idx = get_index(e.to);
    return adjacency_matrix[from_idx][to_idx].has_value() &&
      e.distance == adjacency_matrix[from_idx][to_idx].value();
  }

  std::vector<Edge> edges(const Vertex& vertex) const {
    std::vector<Edge> result;
    if (!has_vertex(vertex)) return result;

    size_t from_idx = get_index(vertex);
    for (size_t i = 0; i < vertices_list.size(); ++i) {
      if (adjacency_matrix[from_idx][i].has_value()) {
        result.push_back({ vertex, vertices_list[i], adjacency_matrix[from_idx][i].value() });
      }
    }
    return result;
  }

  size_t order() const {
    return vertices_list.size();
  }

  size_t degree(const Vertex& v) const {
    if (!has_vertex(v)) return 0;
    size_t from_idx = get_index(v);
    size_t deg = 0;
    for (size_t i = 0; i < vertices_list.size(); ++i) {
      if (adjacency_matrix[from_idx][i].has_value()) deg++;
    }
    return deg;
  }

  bool is_connected() const {
    if (order() == 0) return true;
    if (order() == 1) return true;

    auto dfs = [this](size_t start, std::vector<bool>& visited) {
      std::vector<size_t> stack;
      stack.push_back(start);
      visited[start] = true;

      while (!stack.empty()) {
        size_t u = stack.back();
        stack.pop_back();
        for (size_t v = 0; v < this->order(); ++v) {
          if (this->adjacency_matrix[u][v].has_value() && !visited[v]) {
            visited[v] = true;
            stack.push_back(v);
          }
        }
      }
      };

    for (size_t i = 0; i < order(); ++i) {
      std::vector<bool> visited(order(), false);
      dfs(i, visited);
      for (size_t j = 0; j < order(); ++j) {
        if (!visited[j]) return false;
      }
    }
    return true;
  }

  std::unordered_map<Vertex, std::optional<Distance>> dijkstra(const Vertex& from) const {
    std::unordered_map<Vertex, std::optional<Distance>> distances;
    for (const auto& v : vertices_list) {
      distances[v] = std::nullopt;
    }

    if (!has_vertex(from)) return distances;
    distances[from] = Distance(0);
    std::unordered_set<Vertex> unvisited;
    for (const auto& v : vertices_list) {
      unvisited.insert(v);
    }

    while (!unvisited.empty()) {
      Vertex current;
      bool found = false;
      Distance min_dist = std::numeric_limits<Distance>::max();

      for (const auto& v : unvisited) {
        if (distances[v].has_value() && distances[v].value() < min_dist) {
          min_dist = distances[v].value();
          current = v;
          found = true;
        }
      }

      if (!found) break;

      unvisited.erase(current);

      size_t current_idx = get_index(current);
      for (size_t i = 0; i < vertices_list.size(); ++i) {
        if (adjacency_matrix[current_idx][i].has_value()) {
          const Vertex& neighbor = vertices_list[i];
          Distance edge_weight = adjacency_matrix[current_idx][i].value();

          Distance new_dist = distances[current].value() + edge_weight;

          if (!distances[neighbor].has_value() || new_dist < distances[neighbor].value()) {
            distances[neighbor] = new_dist;
          }
        }
      }
    }

    return distances;
  }

  Distance get_edge_weight(const Vertex& from, const Vertex& to) const {
    size_t from_idx = get_index(from);
    size_t to_idx = get_index(to);
    if (adjacency_matrix[from_idx][to_idx].has_value()) {
      return adjacency_matrix[from_idx][to_idx].value();
    }
    throw std::runtime_error("Edge not found");
  }

  std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
    if (!has_vertex(from) || !has_vertex(to)) return {};

    auto distances = dijkstra(from);
    if (!distances.at(to).has_value()) return {};

    std::vector<Edge> path;
    Vertex current = to;
    std::unordered_set<Vertex> visited;

    while (current != from) {
      if (visited.count(current)) break;
      visited.insert(current);
      bool found = false;

      for (const auto& v : vertices_list) {
        if (has_edge(v, current)) {
          if (distances.at(v).has_value()) {
            Distance edge_dist = get_edge_weight(v, current);
            if (std::abs(distances.at(v).value() + edge_dist - distances.at(current).value()) < 1e-9) {
              path.push_back({ v, current, edge_dist });
              current = v;
              found = true;
              break;
            }
          }
        }
      }
      if (!found) break;
    }

    std::reverse(path.begin(), path.end());
    return path;
  }

  std::vector<Vertex> walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) const {
    if (!has_vertex(start_vertex)) return {};

    std::vector<Vertex> walk_order;
    std::unordered_set<Vertex> visited;
    std::vector<Vertex> stack = { start_vertex };

    while (!stack.empty()) {
      Vertex current = stack.back();
      stack.pop_back();

      if (visited.count(current)) continue;
      visited.insert(current);
      walk_order.push_back(current);
      action(current);

      size_t idx = get_index(current);
      for (size_t i = 0; i < vertices_list.size(); ++i) {
        if (adjacency_matrix[idx][i].has_value()) {
          const Vertex& neighbor = vertices_list[i];
          if (!visited.count(neighbor)) {
            stack.push_back(neighbor);
          }
        }
      }
    }
    return walk_order;
  }

  void export_to_json(const std::string& filename = "graph_data.json") const {
    std::ofstream json_file(filename);
    if (!json_file.is_open()) {
      std::cout << "Error: cannot create " << filename << "\n";
      return;
    }

    std::vector<Vertex> verts = vertices();

    json_file << "{\n";
    json_file << "  \"vertices\": [\n";

    for (size_t i = 0; i < verts.size(); ++i) {
      json_file << "    \"" << verts[i] << "\"";
      if (i < verts.size() - 1) json_file << ",";
      json_file << "\n";
    }

    json_file << "  ],\n";
    json_file << "  \"edges\": [\n";

    bool first_edge = true;
    for (const auto& from : verts) {
      auto edges_list = edges(from);
      for (const auto& e : edges_list) {
        if (!first_edge) json_file << ",\n";
        json_file << "    {\n";
        json_file << "      \"from\": \"" << e.from << "\",\n";
        json_file << "      \"to\": \"" << e.to << "\",\n";
        json_file << "      \"weight\": " << e.distance << "\n";
        json_file << "    }";
        first_edge = false;
      }
    }

    json_file << "\n  ]\n}\n";
    json_file.close();

    std::cout << "Exported graph to JSON: " << filename << "\n";
  }


  void print() const {
    std::cout << "Vertices (" << order() << "): ";
    for (const auto& v : vertices_list) {
      std::cout << v << " ";
    }
    std::cout << "\n\nAdjacency Matrix:\n    ";
    for (const auto& v : vertices_list) {
      std::cout << std::setw(6) << v;
    }
    std::cout << "\n";

    for (size_t i = 0; i < order(); ++i) {
      std::cout << std::setw(4) << vertices_list[i];
      for (size_t j = 0; j < order(); ++j) {
        if (adjacency_matrix[i][j].has_value()) {
          std::cout << std::setw(6) << adjacency_matrix[i][j].value();
        }
        else {
          std::cout << std::setw(6) << " - ";
        }
      }
      std::cout << "\n";
    }

    std::cout << "\nEdges:\n";
    for (size_t i = 0; i < order(); ++i) {
      for (size_t j = 0; j < order(); ++j) {
        if (adjacency_matrix[i][j].has_value()) {
          std::cout << vertices_list[i] << " -> " << vertices_list[j]
            << " (" << adjacency_matrix[i][j].value() << ")\n";
        }
      }
    }
  }
};

template<typename Vertex, typename Distance>
Vertex find_highest_avg_degree_node(const Graph<Vertex, Distance>& g) {
  if (g.order() == 0) throw std::runtime_error("Graph is empty");

  Vertex best_vertex = g.vertices()[0];
  Distance max_avg = -1;

  for (const auto& v : g.vertices()) {
    auto edges = g.edges(v);
    if (edges.empty()) continue;

    Distance sum = 0;
    for (const auto& e : edges) {
      sum += e.distance;
    }
    Distance avg = sum / edges.size();

    if (avg > max_avg) {
      max_avg = avg;
      best_vertex = v;
    }
  }

  return best_vertex;
}

int main() {
  Graph<std::string, double> g;

  // Добавляем вершины (травмпункты)
  g.add_vertex("Hospital_1");
  g.add_vertex("Hospital_2");
  g.add_vertex("Hospital_3");
  g.add_vertex("Hospital_4");
  g.add_vertex("Hospital_5");
  g.add_vertex("Hospital_6");

  // Добавляем рёбра с расстояниями (в километрах)
  g.add_edge("Hospital_1", "Hospital_2", 12.5);
  g.add_edge("Hospital_1", "Hospital_3", 8.3);
  g.add_edge("Hospital_2", "Hospital_3", 5.7);
  g.add_edge("Hospital_2", "Hospital_4", 15.2);
  g.add_edge("Hospital_3", "Hospital_4", 6.8);
  g.add_edge("Hospital_3", "Hospital_5", 9.4);
  g.add_edge("Hospital_4", "Hospital_5", 3.1);
  g.add_edge("Hospital_4", "Hospital_6", 11.9);
  g.add_edge("Hospital_5", "Hospital_6", 4.2);
  g.add_edge("Hospital_5", "Hospital_1", 14.6);
  g.add_edge("Hospital_2", "Hospital_1", 10.3);
  g.add_edge("Hospital_6", "Hospital_2", 7.8);

  g.print();


  std::cout << "\n========== DFS Traversal from Hospital_1 ==========\n";
  std::vector<std::string> walk_result;
  g.walk("Hospital_1", [&](const std::string& v) {
    std::cout << "Visited: " << v << "\n";
    walk_result.push_back(v);
    });
  std::cout << "Traversal order: ";
  for (const auto& v : walk_result) std::cout << v << " ";
  std::cout << "\n";

  std::cout << "\n========== Shortest paths from Hospital_1 (Dijkstra) ==========\n";
  auto dist = g.dijkstra("Hospital_1");
  for (const auto& v : g.vertices()) {
    if (dist.at(v).has_value()) {
      std::cout << "Hospital_1 -> " << v << " = " << dist.at(v).value() << " km\n";
    }
    else {
      std::cout << "Hospital_1 -> " << v << " = unreachable\n";
    }
  }

  std::cout << "\n========== Shortest path from Hospital_1 to Hospital_6 ==========\n";
  auto path = g.shortest_path("Hospital_1", "Hospital_6");
  if (!path.empty()) {
    std::cout << "Hospital_1";
    for (const auto& e : path) {
      std::cout << " --(" << e.distance << " km)--> " << e.to;
    }
    std::cout << "\nTotal distance: " << dist["Hospital_6"].value() << " km\n";
  }
  else {
    std::cout << "Path not found\n";
  }

  std::cout << "\n========== Additional shortest paths ==========\n";

  // Путь от Hospital_2 до Hospital_5
  auto path2 = g.shortest_path("Hospital_2", "Hospital_5");
  if (!path2.empty()) {
    std::cout << "Hospital_2 -> Hospital_5: ";
    std::cout << "Hospital_2";
    for (const auto& e : path2) {
      std::cout << " --(" << e.distance << ")--> " << e.to;
    }
    std::cout << " (Total: " << g.dijkstra("Hospital_2")["Hospital_5"].value() << " km)\n";
  }

  // Путь от Hospital_3 до Hospital_6
  auto path3 = g.shortest_path("Hospital_3", "Hospital_6");
  if (!path3.empty()) {
    std::cout << "Hospital_3 -> Hospital_6: ";
    std::cout << "Hospital_3";
    for (const auto& e : path3) {
      std::cout << " --(" << e.distance << ")--> " << e.to;
    }
    std::cout << " (Total: " << g.dijkstra("Hospital_3")["Hospital_6"].value() << " km)\n";
  }

  std::cout << "\n========== Task: Finding hospital with maximum average distance to neighbors ==========\n";
  try {
    std::string result = find_highest_avg_degree_node(g);
    std::cout << "\n Hospital with maximum average outgoing edge length: " << result << "\n\n";

    std::cout << "Detailed statistics for each hospital:\n";
    std::cout << "=========================================\n";
    for (const auto& v : g.vertices()) {
      auto edges = g.edges(v);
      if (!edges.empty()) {
        double sum = 0;
        int count = 0;
        std::cout << v << ":\n";
        std::cout << "  Outgoing edges: ";
        for (const auto& e : edges) {
          sum += e.distance;
          count++;
          std::cout << "-> " << e.to << " (" << e.distance << " km) ";
        }
        double avg = sum / count;
        std::cout << "\n  Average distance: " << std::fixed << std::setprecision(2) << avg << " km";
        if (v == result) std::cout << " << MAXIMUM >>";
        std::cout << "\n\n";
      }
      else {
        std::cout << v << ": no outgoing edges\n\n";
      }
    }

    // Дополнительная информация о графе
    std::cout << "=========================================\n";
    std::cout << "Graph summary:\n";
    std::cout << "  Total vertices: " << g.order() << "\n";
    std::cout << "  Is connected: " << (g.is_connected() ? "Yes" : "No") << "\n";

    // Вычисляем общее количество рёбер
    int total_edges = 0;
    for (const auto& v : g.vertices()) {
      total_edges += g.edges(v).size();
    }
    std::cout << "  Total edges: " << total_edges << "\n";
  }
  catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << "\n";
  }

  return 0;
}
