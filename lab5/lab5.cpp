/*
 * Лабораторна робота №5
 * Варіант (вибрані завдання):
 * - Блок 0: Матриця суміжності та Структура суміжності
 * - Блок 1: Перевірка зв'язності графу
 * - Блок 2: Обхід графу в ширину (BFS)
 * - Блок 3: Алгоритм Дейкстри
 * - Блок 4: Алгоритм Кана (топологічне сортування)
 * - Блок 5: Побудова кістякового дерева на основі BFS
 * - Блок 6: Алгоритм побудови мінімального кістякового дерева (Kruskal)
 */

// GEMINI reviewer and creator of the comments(debugger also :) )
// If I could get your feedback on using opus 4.7 i would be very grateful
// And if you can give me advice on how to use LLM in real projects,
// namely, what theory for work do you need to know in order to use LLM at work,
//   and to be oriented in what it does. And to give the right prompts, etc.

#pragma execution_character_set("utf-8")

#include <algorithm>
#include <chrono>
#include <clocale>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <queue>
#include <random>
#include <vector>
#define NOMINMAX
#include <windows.h>

using namespace std;

// ---------------- СТРУКТУРИ ДАНИХ ---------------- //

// Константа INF (нескінченність) використовується для позначення відсутності
// ребра між двома вершинами у матриці суміжності. Вона дорівнює максимальному
// значенню для типу int.
const int INF = numeric_limits<int>::max();

struct Edge {
  int u, v;
  int weight;
  bool operator<(const Edge &other) const { return weight < other.weight; }
};

struct Graph {
  bool isMatrix; // true = матриця суміжності, false = список суміжності
  bool oriented;
  int verticesCount;
  int edgesCount;

  // Дані матриці або списку (одне з двох буде використано)
  vector<vector<int>> matrix;
  vector<list<pair<int, int>>> adjList;
};

// ---------------- БАЗОВІ ФУНКЦІЇ ДОСТУПУ (ФУНКЦІОНАЛЬНИЙ ПІДХІД)

Graph createGraph(int verticesCount, bool isOriented, bool isMatrix) {
  Graph g;
  g.isMatrix = isMatrix;
  g.oriented = isOriented;
  g.verticesCount = verticesCount;
  g.edgesCount = 0;

  if (isMatrix) {
    g.matrix.resize(verticesCount, vector<int>(verticesCount, INF));
  } else {
    g.adjList.resize(verticesCount);
  }
  return g;
}

// Передача за посиланням у чистому процедурному стилі мовою C
void addEdge(Graph &g, int u, int v, int weight = 1) {
  if (u < 0 || u >= g.verticesCount || v < 0 || v >= g.verticesCount)
    return;

  if (g.isMatrix) {
    if (g.matrix[u][v] == INF)
      g.edgesCount++;
    g.matrix[u][v] = weight;
    if (!g.oriented) {
      if (g.matrix[v][u] == INF && u != v)
        g.edgesCount++;
      g.matrix[v][u] = weight;
    }
  } else {
    g.adjList[u].push_back({v, weight});
    if (!g.oriented && u != v) {
      g.adjList[v].push_back({u, weight});
    }
    g.edgesCount++;
  }
}

void printGraph(const Graph &g) {
  if (g.isMatrix) {
    cout << "Матриця суміжності (" << g.verticesCount << " вершин):" << endl
         << "  ";
    for (int i = 0; i < g.verticesCount; ++i)
      cout << setw(4) << i;
    cout << endl;
    for (int i = 0; i < g.verticesCount; ++i) {
      cout << i << " ";
      for (int j = 0; j < g.verticesCount; ++j) {
        if (g.matrix[i][j] == INF)
          cout << setw(4) << "-";
        else
          cout << setw(4) << g.matrix[i][j];
      }
      cout << endl;
    }
  } else {
    cout << "Список суміжності (" << g.verticesCount << " вершин):" << endl;
    for (int i = 0; i < g.verticesCount; ++i) {
      cout << "[" << i << "]: ";
      for (auto edge : g.adjList[i]) {
        cout << "->(" << edge.first << ", w:" << edge.second << ") ";
      }
      cout << endl;
    }
  }
}

vector<pair<int, int>> getNeighbors(const Graph &g, int u) {
  vector<pair<int, int>> neighbors;
  if (u < 0 || u >= g.verticesCount)
    return neighbors;

  if (g.isMatrix) {
    for (int v = 0; v < g.verticesCount; ++v) {
      if (g.matrix[u][v] != INF)
        neighbors.push_back({v, g.matrix[u][v]});
    }
  } else {
    for (auto edge : g.adjList[u])
      neighbors.push_back(edge);
  }
  return neighbors;
}

vector<Edge> getEdges(const Graph &g) {
  vector<Edge> edges;
  if (g.isMatrix) {
    for (int i = 0; i < g.verticesCount; ++i) {
      for (int j = (g.oriented ? 0 : i); j < g.verticesCount; ++j) {
        if (g.matrix[i][j] != INF)
          edges.push_back({i, j, g.matrix[i][j]});
      }
    }
  } else {
    for (int u = 0; u < g.verticesCount; ++u) {
      for (auto edge : g.adjList[u]) {
        int v = edge.first;
        if (g.oriented || u <= v)
          edges.push_back({u, v, edge.second});
      }
    }
  }
  return edges;
}

Graph generateRandomGraph(int vertices, int edges, bool oriented,
                          bool isMatrix) {
  Graph graph = createGraph(vertices, oriented, isMatrix);

  int maxEdges =
      oriented ? vertices * (vertices - 1) : vertices * (vertices - 1) / 2;
  if (edges > maxEdges)
    edges = maxEdges;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> distWeight(1, 20);

  vector<Edge> allPossibleEdges;
  for (int i = 0; i < vertices; ++i) {
    for (int j = (oriented ? 0 : i + 1); j < vertices; ++j) {
      if (i != j)
        allPossibleEdges.push_back({i, j, distWeight(gen)});
    }
  }
  shuffle(allPossibleEdges.begin(), allPossibleEdges.end(), gen);

  for (int i = 0; i < edges; ++i) {
    addEdge(graph, allPossibleEdges[i].u, allPossibleEdges[i].v,
            allPossibleEdges[i].weight);
  }
  return graph;
}

Graph convertGraph(const Graph &g) {
  bool targetMatrix = !g.isMatrix;
  Graph newGraph = createGraph(g.verticesCount, g.oriented, targetMatrix);
  for (const auto &edge : getEdges(g))
    addEdge(newGraph, edge.u, edge.v, edge.weight);
  return newGraph;
}

bool isConnected(const Graph &graph) {
  int n = graph.verticesCount;
  if (n == 0)
    return true;
  vector<bool> visited(n, false);
  queue<int> q;
  q.push(0);
  visited[0] = true;
  int count = 1;

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (auto neighbor : getNeighbors(graph, u)) {
      int v = neighbor.first;
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
        count++;
      }
    }
  }
  return count == n;
}

vector<int> bfsTraversal(const Graph &graph, int start, bool sortByWeight) {
  int n = graph.verticesCount;
  vector<int> result;
  if (n == 0 || start < 0 || start >= n)
    return result;

  vector<bool> visited(n, false);
  queue<int> q;
  q.push(start);
  visited[start] = true;

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    result.push_back(u);

    auto neighbors = getNeighbors(graph, u);
    if (sortByWeight) {
      sort(neighbors.begin(), neighbors.end(),
           [](const pair<int, int> &a, const pair<int, int> &b) {
             return a.second < b.second;
           });
    }

    for (auto neighbor : neighbors) {
      int v = neighbor.first;
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
      }
    }
  }
  return result;
}

vector<int> dijkstraAlgorithm(const Graph &graph, int startNode) {
  int n = graph.verticesCount;
  vector<int> dist(n, INF);
  if (n == 0 || startNode < 0 || startNode >= n)
    return dist;

  priority_queue<pair<int, int>, vector<pair<int, int>>,
                 greater<pair<int, int>>>
      pq;
  dist[startNode] = 0;
  pq.push({0, startNode});

  while (!pq.empty()) {
    int d = pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (d > dist[u])
      continue;

    for (auto neighbor : getNeighbors(graph, u)) {
      int v = neighbor.first;
      int weight = neighbor.second;

      if (dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        pq.push({dist[v], v});
      }
    }
  }
  return dist;
}

vector<int> kahnAlgorithm(const Graph &graph) {
  int n = graph.verticesCount;
  vector<int> inDegree(n, 0);
  vector<int> result;

  if (!graph.oriented) {
    cout << "Топологічне сортування неможливе для неорієнтованого графу."
         << endl;
    return result;
  }

  for (int i = 0; i < n; ++i) {
    for (auto neighbor : getNeighbors(graph, i)) {
      inDegree[neighbor.first]++;
    }
  }

  queue<int> q;
  for (int i = 0; i < n; ++i) {
    if (inDegree[i] == 0)
      q.push(i);
  }

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    result.push_back(u);

    for (auto neighbor : getNeighbors(graph, u)) {
      int v = neighbor.first;
      inDegree[v]--;
      if (inDegree[v] == 0)
        q.push(v);
    }
  }

  if (result.size() != n) {
    cout << "Граф містить цикл. Топологічне сортування неможливе." << endl;
    result.clear();
  }
  return result;
}

pair<vector<Edge>, int> spanningTreeBFS(const Graph &graph, int start,
                                        bool sortByWeight) {
  int n = graph.verticesCount;
  vector<Edge> treeEdges;
  int totalWeight = 0;

  if (n == 0 || start < 0 || start >= n)
    return {treeEdges, totalWeight};

  vector<bool> visited(n, false);
  queue<int> q;
  q.push(start);
  visited[start] = true;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    auto neighbors = getNeighbors(graph, u);
    if (sortByWeight) {
      sort(neighbors.begin(), neighbors.end(),
           [](const pair<int, int> &a, const pair<int, int> &b) {
             return a.second < b.second;
           });
    }

    for (auto neighbor : neighbors) {
      int v = neighbor.first;
      int w = neighbor.second;
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
        treeEdges.push_back({u, v, w});
        totalWeight += w;
      }
    }
  }
  return {treeEdges, totalWeight};
}

// Система непересічних множин у вигляді структури та функцій
struct DisjointSet {
  vector<int> parent, rank;
};

DisjointSet createDisjointSet(int n) {
  DisjointSet ds;
  ds.parent.resize(n);
  ds.rank.resize(n, 0);
  for (int i = 0; i < n; ++i)
    ds.parent[i] = i;
  return ds;
}

int dsFind(DisjointSet &ds, int i) {
  if (ds.parent[i] == i)
    return i;
  return ds.parent[i] = dsFind(ds, ds.parent[i]);
}

bool dsUnite(DisjointSet &ds, int i, int j) {
  int root_i = dsFind(ds, i);
  int root_j = dsFind(ds, j);
  if (root_i != root_j) {
    if (ds.rank[root_i] < ds.rank[root_j])
      ds.parent[root_i] = root_j;
    else if (ds.rank[root_i] > ds.rank[root_j])
      ds.parent[root_j] = root_i;
    else {
      ds.parent[root_j] = root_i;
      ds.rank[root_i]++;
    }
    return true;
  }
  return false;
}

pair<vector<Edge>, int> kruskalMST(const Graph &graph) {
  int n = graph.verticesCount;
  vector<Edge> treeEdges;
  int totalWeight = 0;

  vector<Edge> allEdges = getEdges(graph);
  sort(allEdges.begin(), allEdges.end());

  DisjointSet ds = createDisjointSet(n);

  for (const auto &edge : allEdges) {
    if (dsUnite(ds, edge.u, edge.v)) {
      treeEdges.push_back(edge);
      totalWeight += edge.weight;
    }
  }
  return {treeEdges, totalWeight};
}

// ---------------- UI & РЕЖИМИ ---------------- //

void runAlgorithmsShowcase(const Graph &graph, int startNode) {
  cout << endl << "=== Обхід графу в ширину (BFS) ===" << endl;
  cout << "Довільний порядок сусідів: ";
  for (int v : bfsTraversal(graph, startNode, false))
    cout << v << " ";
  cout << endl << "За зростанням ваги ребра:  ";
  for (int v : bfsTraversal(graph, startNode, true))
    cout << v << " ";
  cout << endl;

  cout << endl << "=== Перевірка зв'язності ===" << endl;
  cout << "Граф зв'язний: " << (isConnected(graph) ? "Так" : "Ні") << endl;

  cout << endl << "=== Алгоритм Дейкстри ===" << endl;
  cout << "Найкоротші шляхи від вершини " << startNode << ":" << endl;
  auto dists = dijkstraAlgorithm(graph, startNode);
  for (int i = 0; i < dists.size(); ++i) {
    if (dists[i] == INF)
      cout << "До " << i << " : шлях відсутній" << endl;
    else
      cout << "До " << i << " : " << dists[i] << endl;
  }

  if (graph.oriented) {
    cout << endl << "=== Алгоритм Кана (топологічне сортування) ===" << endl;
    auto sorted = kahnAlgorithm(graph);
    if (!sorted.empty()) {
      cout << "Результат: ";
      for (int v : sorted)
        cout << v << " ";
      cout << endl;
    }
  }

  cout << endl << "=== Кістякове дерево (BFS) ===" << endl;
  auto tree = spanningTreeBFS(graph, startNode, true);
  for (auto e : tree.first)
    cout << e.u << " - " << e.v << " (вага: " << e.weight << ")" << endl;
  cout << "Сумарна вага: " << tree.second << endl;

  if (!graph.oriented) {
    cout << endl << "=== Мінімальне кістякове дерево (Kruskal) ===" << endl;
    auto mst = kruskalMST(graph);
    for (auto e : mst.first)
      cout << e.u << " - " << e.v << " (вага: " << e.weight << ")" << endl;
    cout << "Сумарна вага: " << mst.second << endl;
  }
}

void interactiveMode() {
  cout << endl << "[Інтерактивний режим]" << endl;
  int vCount, eCount, type, repr;
  cout << "Кількість вершин: ";
  if (!(cin >> vCount))
    return;
  cout << "Кількість ребер (для генерації випадкового графу): ";
  if (!(cin >> eCount))
    return;
  cout << "Орієнтований (1 - так, 0 - ні)? ";
  if (!(cin >> type))
    return;
  cout << "Подання (1 - Матриця, 2 - Список суміжності)? ";
  if (!(cin >> repr))
    return;

  Graph graph = generateRandomGraph(vCount, eCount, type == 1, repr == 1);

  int act;
  do {
    cout << endl
         << "1. Вивести граф" << endl
         << "2. Додати ребро" << endl
         << "3. Виконати алгоритми" << endl
         << "0. Назад" << endl
         << "Оберіть дію: ";
    if (!(cin >> act)) {
      cin.clear();
      cin.ignore(10000, '\n');
      break;
    }
    if (act == 1)
      printGraph(graph);
    else if (act == 2) {
      int u, v, w;
      cout << "u v weight: ";
      cin >> u >> v >> w;
      addEdge(graph, u, v, w);
    } else if (act == 3) {
      int start;
      cout << "Стартова вершина: ";
      cin >> start;
      runAlgorithmsShowcase(graph, start);
    }
  } while (act != 0);
}

void demoMode() {
  cout << endl << "[Демонстраційний режим]" << endl;
  cout << "Генерація неорієнтованого графу (матриця суміжності) на 6 вершин..."
       << endl;
  Graph graph = generateRandomGraph(6, 9, false, true);
  printGraph(graph);
  runAlgorithmsShowcase(graph, 0);
}

void benchmarkMode() {
  cout << endl << "[Автоматичний режим Benchmark]" << endl;
  int v = 500, e = 5000;
  cout << "Генерація графів: " << v << " вершин, " << e << " ребер..." << endl;

  auto t1 = chrono::high_resolution_clock::now();
  Graph mGraph = generateRandomGraph(v, e, false, true);
  auto t2 = chrono::high_resolution_clock::now();
  Graph lGraph = generateRandomGraph(v, e, false, false);
  auto t3 = chrono::high_resolution_clock::now();

  cout << "Час створення (Матриця):  "
       << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms"
       << endl;
  cout << "Час створення (Список): "
       << chrono::duration_cast<chrono::milliseconds>(t3 - t2).count() << " ms"
       << endl;

  auto measureDijkstra = [](const Graph &g) {
    auto start = chrono::high_resolution_clock::now();
    dijkstraAlgorithm(g, 0);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
  };

  auto measureKruskal = [](const Graph &g) {
    auto start = chrono::high_resolution_clock::now();
    kruskalMST(g);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
  };

  cout << "Dijkstra (Матриця):  " << measureDijkstra(mGraph) << " мікросекунд"
       << endl;
  cout << "Dijkstra (Список): " << measureDijkstra(lGraph) << " мікросекунд"
       << endl;

  cout << "Kruskal (Матриця):  " << measureKruskal(mGraph) << " мікросекунд"
       << endl;
  cout << "Kruskal (Список): " << measureKruskal(lGraph) << " мікросекунд"
       << endl;
}

int main() {
  // Налаштування консолі Windows для коректного відображення кирилиці (UTF-8)
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  setlocale(LC_ALL, ".UTF-8");

  int choice;
  do {
    cout << endl << "=== ЛАБОРАТОРНА РОБОТА 5 ===" << endl;
    cout << "1. Інтерактивний режим" << endl;
    cout << "2. Демонстраційний режим" << endl;
    cout << "3. Режим Benchmark (автоматичний)" << endl;
    cout << "0. Вихід" << endl;
    cout << "Виберіть опцію: ";
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(10000, '\n');
      break;
    }

    switch (choice) {
    case 1:
      interactiveMode();
      break;
    case 2:
      demoMode();
      break;
    case 3:
      benchmarkMode();
      break;
    case 0:
      cout << "Вихід..." << endl;
      break;
    default:
      cout << "Неправильний вибір." << endl;
    }
  } while (choice != 0);

  return 0;
}
