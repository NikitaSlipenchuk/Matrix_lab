#!/usr/bin/env python3
# graph_visualizer.py

import matplotlib.pyplot as plt
import networkx as nx

# Создаём граф
G = nx.DiGraph()

# Добавляем вершины
vertices = ['A', 'B', 'C', 'D', 'E']
G.add_nodes_from(vertices)

# Добавляем рёбра (from, to, weight)
edges = [
    ('A', 'B', 5.0),
    ('A', 'C', 3.0),
    ('B', 'C', 2.0),
    ('B', 'D', 7.0),
    ('C', 'D', 4.0),
    ('C', 'E', 6.0),
    ('D', 'E', 1.0),
    ('E', 'A', 8.0),
    ('B', 'A', 2.0),
]

for from_v, to_v, weight in edges:
    G.add_edge(from_v, to_v, weight=weight)

# Позиции вершин (ручная расстановка для красивого вида)
pos = {
    'A': (0, 0),
    'B': (2, 2),
    'C': (1, 0),
    'D': (3, 1),
    'E': (4, 0)
}

# Рисуем граф
plt.figure(figsize=(10, 7))

# Рисуем вершины
nx.draw_networkx_nodes(G, pos, node_color='lightblue', node_size=2000, edgecolors='black', linewidths=2)

# Рисуем рёбра
nx.draw_networkx_edges(G, pos, edge_color='gray', arrows=True, arrowsize=20, width=1.5)

# Подписи вершин
nx.draw_networkx_labels(G, pos, font_size=14, font_weight='bold')

# Подписи весов рёбер
edge_labels = {(u, v): f'{d["weight"]}' for u, v, d in G.edges(data=True)}
nx.draw_networkx_edge_labels(G, pos, edge_labels, font_size=10)

plt.title('Граф транспортных узлов', fontsize=16, fontweight='bold')
plt.axis('off')
plt.tight_layout()
plt.show()