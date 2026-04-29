/*
 * Лабораторна робота №6 – Структури даних та алгоритми на графах
 * Варіант: Блок 0 (завдання 1, 2) + Блок 1 (завд. 1,2) + Блок 2 (завд. 1,3)
 *          + Блок 3 (Dijkstra) + Блок 4 (Kahn) + Блок 5 (BFS-дерево)
 *          + Блок 6 (Kruskal)
 * Візуалізація: вивід у формат Graphviz DOT (файл graph.dot).
 * http://www.webgraphviz.com/
 */

#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// Force MSVC to use UTF-8 for string literals
#if defined(_MSC_VER)
#pragma execution_character_set("utf-8")
#endif

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
//  КОНСТАНТИ
// ─────────────────────────────────────────────────────────────────────────────
const double INF = numeric_limits<double>::infinity();
const double NO_EDGE = INF; // у матриці суміжності: немає ребра

// ─────────────────────────────────────────────────────────────────────────────
//  ПОДАННЯ 1 – МАТРИЦЯ СУМІЖНОСТІ
//  AdjMatrix: vector<vector<double>>, розмір n×n
//  NO_EDGE означає відсутність ребра, інше значення – вага.
// ─────────────────────────────────────────────────────────────────────────────

// Створення порожньої матриці суміжності
vector<vector<double>> mat_create(int n) {
  return vector<vector<double>>(n, vector<double>(n, NO_EDGE));
}

// Додавання ребра
void mat_add_edge(vector<vector<double>> &m, int u, int v, double w,
                  bool directed) {
  m[u][v] = w;
  if (!directed)
    m[v][u] = w;
}

// Видалення ребра
void mat_remove_edge(vector<vector<double>> &m, int u, int v, bool directed) {
  m[u][v] = NO_EDGE;
  if (!directed)
    m[v][u] = NO_EDGE;
}

// Кількість вершин
int mat_n(const vector<vector<double>> &m) { return (int)m.size(); }

// Вивід матриці у консоль
void mat_print(const vector<vector<double>> &m,
               const string &label = "AdjMatrix") {
  int n = mat_n(m);
  cout << endl << "=== " << label << " (" << n << " вершин) ===" << endl;
  cout << "     ";
  for (int i = 0; i < n; ++i)
    cout << setw(6) << i;
  cout << endl;
  for (int i = 0; i < n; ++i) {
    cout << setw(4) << i << " ";
    for (int j = 0; j < n; ++j) {
      if (m[i][j] == NO_EDGE)
        cout << setw(6) << ".";
      else
        cout << setw(6) << m[i][j];
    }
    cout << endl;
  }
}

// Генерація випадкового графу (матриця)
vector<vector<double>> mat_random(int n, int edges, bool directed,
                                  double wmin = 1.0, double wmax = 10.0) {
  mt19937 rng(42);
  uniform_int_distribution<int> vi(0, n - 1);
  uniform_real_distribution<double> wd(wmin, wmax);
  auto m = mat_create(n);
  int added = 0, maxTries = edges * 10;
  while (added < edges && maxTries-- > 0) {
    int u = vi(rng), v = vi(rng);
    if (u != v && m[u][v] == NO_EDGE) {
      mat_add_edge(m, u, v, round(wd(rng) * 10) / 10.0, directed);
      ++added;
    }
  }
  return m;
}

// ─────────────────────────────────────────────────────────────────────────────
//  ПОДАННЯ 2 – СПИСОК СУМІЖНОСТІ
//  AdjList: vector<list<pair<int,double>>>
//  pair: (сусід, вага)
// ─────────────────────────────────────────────────────────────────────────────

using AdjList = vector<list<pair<int, double>>>;

AdjList list_create(int n) { return AdjList(n); }

void list_add_edge(AdjList &g, int u, int v, double w, bool directed) {
  g[u].push_back({v, w});
  if (!directed)
    g[v].push_back({u, w});
}

void list_remove_edge(AdjList &g, int u, int v, bool directed) {
  g[u].remove_if([v](auto &p) { return p.first == v; });
  if (!directed)
    g[v].remove_if([u](auto &p) { return p.first == u; });
}

int list_n(const AdjList &g) { return (int)g.size(); }

void list_print(const AdjList &g, const string &label = "AdjList") {
  int n = list_n(g);
  cout << endl << "=== " << label << " (" << n << " вершин) ===" << endl;
  for (int i = 0; i < n; ++i) {
    cout << "  " << i << " -> ";
    bool first = true;
    for (auto &[v, w] : g[i]) {
      if (!first)
        cout << ", ";
      cout << v << "(w=" << w << ")";
      first = false;
    }
    if (g[i].empty())
      cout << "(немає сусідів)";
    cout << endl;
  }
}

AdjList list_random(int n, int edges, bool directed, double wmin = 1.0,
                    double wmax = 10.0) {
  mt19937 rng(42);
  uniform_int_distribution<int> vi(0, n - 1);
  uniform_real_distribution<double> wd(wmin, wmax);
  auto g = list_create(n);
  // щоб уникнути дублікатів використовуємо тимчасову матрицю флагів
  vector<vector<bool>> used(n, vector<bool>(n, false));
  int added = 0, maxTries = edges * 10;
  while (added < edges && maxTries-- > 0) {
    int u = vi(rng), v = vi(rng);
    if (u != v && !used[u][v]) {
      double w = round(wd(rng) * 10) / 10.0;
      list_add_edge(g, u, v, w, directed);
      used[u][v] = true;
      if (!directed)
        used[v][u] = true;
      ++added;
    }
  }
  return g;
}

// ─────────────────────────────────────────────────────────────────────────────
//  ПЕРЕТВОРЕННЯ МІЖ ПОДАННЯМИ
// ─────────────────────────────────────────────────────────────────────────────

AdjList mat_to_list(const vector<vector<double>> &m) {
  int n = mat_n(m);
  auto g = list_create(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      if (m[i][j] != NO_EDGE)
        g[i].push_back({j, m[i][j]});
  return g;
}

vector<vector<double>> list_to_mat(const AdjList &g) {
  int n = list_n(g);
  auto m = mat_create(n);
  for (int i = 0; i < n; ++i)
    for (auto &[v, w] : g[i])
      m[i][v] = w;
  return m;
}

// ─────────────────────────────────────────────────────────────────────────────
//  БЛОК 1 – ЗВ'ЯЗНІСТЬ ТА КОМПОНЕНТИ (для неорієнтованого графу)
//  Реалізовано для обох подань.
// ─────────────────────────────────────────────────────────────────────────────

// --- DFS-відвідувач (список суміжності) ---
void dfs_visit(const AdjList &g, int u, vector<bool> &visited, int comp) {
  visited[u] = true;
  for (auto &[v, w] : g[u])
    if (!visited[v])
      dfs_visit(g, v, visited, comp);
}

// Кількість компонент зв'язності (список)
int list_components(const AdjList &g, vector<int> &comp_id) {
  int n = list_n(g);
  vector<bool> visited(n, false);
  comp_id.assign(n, -1);
  int comp = 0;
  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      // BFS для визначення компоненти
      queue<int> q;
      q.push(i);
      visited[i] = true;
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        comp_id[u] = comp;
        for (auto &[v, w] : g[u])
          if (!visited[v]) {
            visited[v] = true;
            q.push(v);
          }
      }
      ++comp;
    }
  }
  return comp;
}

bool list_is_connected(const AdjList &g) {
  vector<int> comp;
  return list_components(g, comp) == 1;
}

// --- Матриця суміжності ---
void mat_dfs_visit(const vector<vector<double>> &m, int u,
                   vector<bool> &visited) {
  int n = mat_n(m);
  visited[u] = true;
  for (int v = 0; v < n; ++v)
    if (m[u][v] != NO_EDGE && !visited[v])
      mat_dfs_visit(m, v, visited);
}

int mat_components(const vector<vector<double>> &m, vector<int> &comp_id) {
  int n = mat_n(m);
  vector<bool> visited(n, false);
  comp_id.assign(n, -1);
  int comp = 0;
  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      // BFS
      queue<int> q;
      q.push(i);
      visited[i] = true;
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        comp_id[u] = comp;
        for (int v = 0; v < n; ++v)
          if (m[u][v] != NO_EDGE && !visited[v]) {
            visited[v] = true;
            q.push(v);
          }
      }
      ++comp;
    }
  }
  return comp;
}

bool mat_is_connected(const vector<vector<double>> &m) {
  vector<int> comp;
  return mat_components(m, comp) == 1;
}

// ─────────────────────────────────────────────────────────────────────────────
//  БЛОК 2 – ОБХОДИ ГРАФУ
// ─────────────────────────────────────────────────────────────────────────────

// DFS рекурсивний (список суміжності), сусіди у довільному порядку
vector<int> list_dfs(const AdjList &g, int start) {
  int n = list_n(g);
  vector<bool> visited(n, false);
  vector<int> order;
  function<void(int)> dfs = [&](int u) {
    visited[u] = true;
    order.push_back(u);
    for (auto &[v, w] : g[u])
      if (!visited[v])
        dfs(v);
  };
  dfs(start);
  return order;
}

// DFS рекурсивний (список), сусіди від найменшої до найбільшої ваги
vector<int> list_dfs_sorted(const AdjList &g, int start) {
  int n = list_n(g);
  vector<bool> visited(n, false);
  vector<int> order;
  function<void(int)> dfs = [&](int u) {
    visited[u] = true;
    order.push_back(u);
    vector<pair<double, int>> neigh;
    for (auto &[v, w] : g[u])
      neigh.push_back({w, v});
    sort(neigh.begin(), neigh.end());
    for (auto &[w, v] : neigh)
      if (!visited[v])
        dfs(v);
  };
  dfs(start);
  return order;
}

// DFS рекурсивний (матриця суміжності)
vector<int> mat_dfs(const vector<vector<double>> &m, int start) {
  int n = mat_n(m);
  vector<bool> visited(n, false);
  vector<int> order;
  function<void(int)> dfs = [&](int u) {
    visited[u] = true;
    order.push_back(u);
    for (int v = 0; v < n; ++v)
      if (m[u][v] != NO_EDGE && !visited[v])
        dfs(v);
  };
  dfs(start);
  return order;
}

// DFS матриця, сусіди від найменшої до найбільшої ваги
vector<int> mat_dfs_sorted(const vector<vector<double>> &m, int start) {
  int n = mat_n(m);
  vector<bool> visited(n, false);
  vector<int> order;
  function<void(int)> dfs = [&](int u) {
    visited[u] = true;
    order.push_back(u);
    vector<pair<double, int>> neigh;
    for (int v = 0; v < n; ++v)
      if (m[u][v] != NO_EDGE)
        neigh.push_back({m[u][v], v});
    sort(neigh.begin(), neigh.end());
    for (auto &[w, v] : neigh)
      if (!visited[v])
        dfs(v);
  };
  dfs(start);
  return order;
}

// BFS (список суміжності), довільний порядок сусідів
vector<int> list_bfs(const AdjList &g, int start) {
  int n = list_n(g);
  vector<bool> visited(n, false);
  vector<int> order;
  queue<int> q;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    order.push_back(u);
    for (auto &[v, w] : g[u])
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
      }
  }
  return order;
}

// BFS (список), сусіди від найменшої до найбільшої ваги
vector<int> list_bfs_sorted(const AdjList &g, int start) {
  int n = list_n(g);
  vector<bool> visited(n, false);
  vector<int> order;
  queue<int> q;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    order.push_back(u);
    vector<pair<double, int>> neigh;
    for (auto &[v, w] : g[u])
      neigh.push_back({w, v});
    sort(neigh.begin(), neigh.end());
    for (auto &[w, v] : neigh)
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
      }
  }
  return order;
}

// BFS (матриця суміжності)
vector<int> mat_bfs(const vector<vector<double>> &m, int start) {
  int n = mat_n(m);
  vector<bool> visited(n, false);
  vector<int> order;
  queue<int> q;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    order.push_back(u);
    for (int v = 0; v < n; ++v)
      if (m[u][v] != NO_EDGE && !visited[v]) {
        visited[v] = true;
        q.push(v);
      }
  }
  return order;
}

// BFS матриця, відсортовані сусіди
vector<int> mat_bfs_sorted(const vector<vector<double>> &m, int start) {
  int n = mat_n(m);
  vector<bool> visited(n, false);
  vector<int> order;
  queue<int> q;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    order.push_back(u);
    vector<pair<double, int>> neigh;
    for (int v = 0; v < n; ++v)
      if (m[u][v] != NO_EDGE)
        neigh.push_back({m[u][v], v});
    sort(neigh.begin(), neigh.end());
    for (auto &[w, v] : neigh)
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
      }
  }
  return order;
}

// Вивід вектора порядку обходу
void print_order(const vector<int> &order, const string &label) {
  cout << label << ": ";
  for (int i = 0; i < (int)order.size(); ++i) {
    if (i)
      cout << " -> ";
    cout << order[i];
  }
  cout << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
//  БЛОК 3 – АЛГОРИТМ ДЕЙКСТРИ
// ─────────────────────────────────────────────────────────────────────────────

struct DijkstraResult {
  vector<double> dist;
  vector<int> prev;
};

// Dijkstra для списку суміжності
DijkstraResult list_dijkstra(const AdjList &g, int src) {
  int n = list_n(g);
  DijkstraResult res;
  res.dist.assign(n, INF);
  res.prev.assign(n, -1);
  res.dist[src] = 0.0;
  // priority_queue: (відстань, вершина), мінімум зверху
  priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
  pq.push({0.0, src});
  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();
    if (d > res.dist[u])
      continue;
    for (auto &[v, w] : g[u]) {
      if (w < 0)
        continue; // Dijkstra не підтримує від'ємні ваги
      double nd = res.dist[u] + w;
      if (nd < res.dist[v]) {
        res.dist[v] = nd;
        res.prev[v] = u;
        pq.push({nd, v});
      }
    }
  }
  return res;
}

// Dijkstra для матриці суміжності
DijkstraResult mat_dijkstra(const vector<vector<double>> &m, int src) {
  return list_dijkstra(mat_to_list(m), src);
}

// Відновлення шляху
vector<int> dijkstra_path(const DijkstraResult &res, int dst) {
  vector<int> path;
  for (int v = dst; v != -1; v = res.prev[v])
    path.push_back(v);
  reverse(path.begin(), path.end());
  if (path.front() != dst && res.dist[dst] == INF)
    return {}; // недосяжна
  return path;
}

void print_dijkstra(const DijkstraResult &res, int src, int n) {
  cout << "  Dijkstra від вершини " << src << ":" << endl;
  for (int v = 0; v < n; ++v) {
    cout << "    " << src << " -> " << v << ": ";
    if (res.dist[v] == INF)
      cout << "недосяжно";
    else {
      cout << res.dist[v] << "  (шлях: ";
      auto path = dijkstra_path(res, v);
      for (int i = 0; i < (int)path.size(); ++i) {
        if (i)
          cout << "->";
        cout << path[i];
      }
      cout << ")";
    }
    cout << endl;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
//  БЛОК 4 – ТОПОЛОГІЧНЕ СОРТУВАННЯ (АЛГОРИТМ КАНА)
// ─────────────────────────────────────────────────────────────────────────────

// Kahn для списку суміжності (орієнтований граф)
// Повертає {} якщо є цикл (топологічний порядок неможливий)
vector<int> list_kahn(const AdjList &g) {
  int n = list_n(g);
  vector<int> indeg(n, 0);
  for (int u = 0; u < n; ++u)
    for (auto &[v, w] : g[u])
      ++indeg[v];
  queue<int> q;
  for (int i = 0; i < n; ++i)
    if (indeg[i] == 0)
      q.push(i);
  vector<int> order;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    order.push_back(u);
    for (auto &[v, w] : g[u])
      if (--indeg[v] == 0)
        q.push(v);
  }
  if ((int)order.size() != n)
    return {}; // є цикл
  return order;
}

vector<int> mat_kahn(const vector<vector<double>> &m) {
  return list_kahn(mat_to_list(m));
}

// ─────────────────────────────────────────────────────────────────────────────
//  БЛОК 5 – КІСТЯКОВЕ ДЕРЕВО ЧЕРЕЗ BFS
// ─────────────────────────────────────────────────────────────────────────────

struct SpanTree {
  vector<tuple<int, int, double>> edges; // (u, v, weight)
  double total_weight;
};

SpanTree list_bfs_span_tree(const AdjList &g, int start) {
  int n = list_n(g);
  vector<bool> visited(n, false);
  SpanTree st;
  st.total_weight = 0.0;
  queue<int> q;
  visited[start] = true;
  q.push(start);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (auto &[v, w] : g[u]) {
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
        st.edges.push_back({u, v, w});
        st.total_weight += w;
      }
    }
  }
  return st;
}

SpanTree mat_bfs_span_tree(const vector<vector<double>> &m, int start) {
  return list_bfs_span_tree(mat_to_list(m), start);
}

void print_span_tree(const SpanTree &st, const string &label) {
  cout << label << " (сумарна вага = " << st.total_weight << "):" << endl;
  for (auto &[u, v, w] : st.edges)
    cout << "  " << u << " -- " << v << "  (w=" << w << ")" << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
//  БЛОК 6 – МІНІМАЛЬНЕ КІСТЯКОВЕ ДЕРЕВО (КРУСКАЛ)
// ─────────────────────────────────────────────────────────────────────────────

// Структура Union-Find
struct UF {
  vector<int> parent, rank_;
};

UF uf_create(int n) {
  UF uf;
  uf.parent.resize(n);
  iota(uf.parent.begin(), uf.parent.end(), 0);
  uf.rank_.assign(n, 0);
  return uf;
}

int uf_find(UF &uf, int x) {
  if (uf.parent[x] != x)
    uf.parent[x] = uf_find(uf, uf.parent[x]);
  return uf.parent[x];
}

bool uf_union(UF &uf, int x, int y) {
  int px = uf_find(uf, x), py = uf_find(uf, y);
  if (px == py)
    return false;
  if (uf.rank_[px] < uf.rank_[py])
    swap(px, py);
  uf.parent[py] = px;
  if (uf.rank_[px] == uf.rank_[py])
    ++uf.rank_[px];
  return true;
}

SpanTree list_kruskal(const AdjList &g) {
  int n = list_n(g);
  // Зібрати всі ребра
  vector<tuple<double, int, int>> edges;
  for (int u = 0; u < n; ++u)
    for (auto &[v, w] : g[u])
      if (u < v)
        edges.push_back({w, u, v}); // уникаємо дублів
  sort(edges.begin(), edges.end());
  auto uf = uf_create(n);
  SpanTree mst;
  mst.total_weight = 0.0;
  for (auto &[w, u, v] : edges) {
    if (uf_union(uf, u, v)) {
      mst.edges.push_back({u, v, w});
      mst.total_weight += w;
    }
  }
  return mst;
}

SpanTree mat_kruskal(const vector<vector<double>> &m) {
  return list_kruskal(mat_to_list(m));
}

// ─────────────────────────────────────────────────────────────────────────────
//  ВІЗУАЛІЗАЦІЯ
// ─────────────────────────────────────────────────────────────────────────────

// Вивід у формат Graphviz DOT
void export_dot(const AdjList &g, const string &filename, bool directed,
                const SpanTree *highlight = nullptr,
                const string &graph_name = "G") {
  ofstream f(filename);
  if (!f) {
    cout << "Не вдалося відкрити " << filename << endl;
    return;
  }
  int n = list_n(g);
  string etype = directed ? "->" : "--";
  string gtype = directed ? "digraph" : "graph";
  f << gtype << " " << graph_name << " {" << endl;
  f << "  rankdir=LR;" << endl;
  f << "  node [shape=circle, style=filled, fillcolor=lightblue];" << endl;
  // ребра, що підсвічуються (MST / span tree)
  set<pair<int, int>> hi;
  if (highlight)
    for (auto &[u, v, w] : highlight->edges) {
      hi.insert({u, v});
      hi.insert({v, u});
    }
  for (int u = 0; u < n; ++u) {
    for (auto &[v, w] : g[u]) {
      if (!directed && u > v)
        continue; // для неорієнтованих – одне ребро
      bool is_hi = hi.count({u, v}) || hi.count({v, u});
      f << "  " << u << " " << etype << " " << v << " [label=\"" << w << "\"";
      if (is_hi)
        f << ", color=red, penwidth=2.5";
      f << "];" << endl;
    }
  }
  f << "}" << endl;
  f.close();
  cout << "  [DOT] Граф збережено у файл: " << filename << endl;
  cout << "  [DOT] Для генерації зображення: dot -Tpng " << filename
       << " -o graph.png" << endl;
}

// Проста ASCII-візуалізація матриці суміжності (матриця вже є ASCII-виводом)
void ascii_list_print(const AdjList &g, const string &label) {
  cout << endl << "=== ASCII граф: " << label << " ===" << endl;
  int n = list_n(g);
  for (int u = 0; u < n; ++u) {
    cout << "  [" << u << "]";
    for (auto &[v, w] : g[u])
      cout << " ---(" << w << ")---> [" << v << "]";
    cout << endl;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
//  BENCHMARK
// ─────────────────────────────────────────────────────────────────────────────

template <typename F> double benchmark_ms(F func, int reps = 3) {
  auto t0 = chrono::high_resolution_clock::now();
  for (int i = 0; i < reps; ++i)
    func();
  auto t1 = chrono::high_resolution_clock::now();
  return chrono::duration<double, milli>(t1 - t0).count() / reps;
}

size_t mat_mem(int n) { return n * n * sizeof(double); }

size_t list_mem_estimate(const AdjList &g) {
  size_t s = g.size() * sizeof(list<pair<int, double>>);
  for (auto &l : g)
    s += l.size() * sizeof(pair<int, double>);
  return s;
}

void run_benchmark() {
  cout << endl
       << "╔══════════════════════════════════════════════════════════════╗"
       << endl;
  cout << "║              BENCHMARK MODE                                  ║"
       << endl;
  cout << "╚══════════════════════════════════════════════════════════════╝"
       << endl;

  vector<int> sizes = {10, 50, 100, 200};
  cout << endl
       << left << setw(8) << "N" << setw(10) << "Ребра" << setw(18)
       << "AdjMatrix пам'ять" << setw(18) << "AdjList пам'ять" << setw(16)
       << "BFS(mat) мс" << setw(16) << "BFS(list) мс" << setw(18)
       << "Dijkstra(list) мс" << setw(18) << "Kruskal мс" << endl;
  cout << string(122, '-') << endl;

  for (int n : sizes) {
    int e = min(n * (n - 1) / 2, n * 3);
    auto mat = mat_random(n, e, false);
    auto lst = mat_to_list(mat);

    double t_bfs_mat = benchmark_ms([&] { mat_bfs(mat, 0); });
    double t_bfs_lst = benchmark_ms([&] { list_bfs(lst, 0); });
    double t_dijk = benchmark_ms([&] { list_dijkstra(lst, 0); });
    double t_kruskal = benchmark_ms([&] { list_kruskal(lst); });

    size_t mem_mat = mat_mem(n);
    size_t mem_list = list_mem_estimate(lst);

    cout << setw(8) << n << setw(10) << e << setw(15) << mem_mat / 1024 << " KB"
         << "   " << setw(15) << mem_list / 1024 << " KB"
         << "   " << setw(13) << fixed << setprecision(4) << t_bfs_mat
         << setw(13) << t_bfs_lst << setw(15) << t_dijk << setw(15) << t_kruskal
         << endl;
  }
  cout << endl
       << "Примітка: пам'ять матриці = N²×8 байт; пам'ять списку залежить "
          "від кількості ребер."
       << endl;
}

// ─────────────────────────────────────────────────────────────────────────────
//  ДЕМОНСТРАЦІЙНИЙ РЕЖИМ
// ─────────────────────────────────────────────────────────────────────────────

void run_demo() {
  cout << endl
       << "╔══════════════════════════════════════════════════════════════╗"
       << endl;
  cout << "║              ДЕМОНСТРАЦІЙНИЙ РЕЖИМ                           ║"
       << endl;
  cout << "╚══════════════════════════════════════════════════════════════╝"
       << endl;

  // Граф:  0-1(4), 0-2(1), 1-3(1), 2-1(2), 2-3(5), 3-4(3)  неорієнтований
  //        Зв'язний, 5 вершин, 6 ребер
  const int N = 5;
  cout << endl
       << "--- Тест 1: Зв'язний неорієнтований граф (5 вершин, 6 ребер) ---"
       << endl;
  auto m = mat_create(N);
  mat_add_edge(m, 0, 1, 4.0, false);
  mat_add_edge(m, 0, 2, 1.0, false);
  mat_add_edge(m, 1, 3, 1.0, false);
  mat_add_edge(m, 2, 1, 2.0, false);
  mat_add_edge(m, 2, 3, 5.0, false);
  mat_add_edge(m, 3, 4, 3.0, false);
  mat_print(m, "Матриця суміжності (неорієнт.)");

  auto g = mat_to_list(m);
  ascii_list_print(g, "Список суміжності (неорієнт.)");

  // Зв'язність
  cout << endl << "[Зв'язність]" << endl;
  cout << "  Матриця: граф "
       << (mat_is_connected(m) ? "зв'язний" : "незв'язний") << endl;
  cout << "  Список:  граф "
       << (list_is_connected(g) ? "зв'язний" : "незв'язний") << endl;

  // Компоненти
  {
    vector<int> comp;
    int nc = list_components(g, comp);
    cout << "  Кількість компонент: " << nc << endl;
    cout << "  Вершина -> компонента: ";
    for (int i = 0; i < N; ++i)
      cout << i << ":" << comp[i] << " ";
    cout << endl;
  }

  // DFS / BFS
  cout << endl << "[Обходи від вершини 0]" << endl;
  print_order(mat_dfs(m, 0), "  DFS (матриця, довільн.)");
  print_order(mat_dfs_sorted(m, 0), "  DFS (матриця, за вагою)");
  print_order(list_dfs(g, 0), "  DFS (список,  довільн.)");
  print_order(list_dfs_sorted(g, 0), "  DFS (список,  за вагою)");
  print_order(mat_bfs(m, 0), "  BFS (матриця, довільн.)");
  print_order(list_bfs(g, 0), "  BFS (список,  довільн.)");
  print_order(list_bfs_sorted(g, 0), "  BFS (список,  за вагою)");

  // Dijkstra
  cout << endl << "[Dijkstra]" << endl;
  auto dr_list = list_dijkstra(g, 0);
  print_dijkstra(dr_list, 0, N);

  // BFS кістякове дерево
  cout << endl << "[BFS кістякове дерево від вершини 0]" << endl;
  auto st = list_bfs_span_tree(g, 0);
  print_span_tree(st, "  BFS Span Tree (список)");
  auto st_m = mat_bfs_span_tree(m, 0);
  print_span_tree(st_m, "  BFS Span Tree (матриця)");

  // Kruskal MST
  cout << endl << "[Kruskal MST]" << endl;
  auto mst = list_kruskal(g);
  print_span_tree(mst, "  MST Kruskal (список)");
  auto mst_m = mat_kruskal(m);
  print_span_tree(mst_m, "  MST Kruskal (матриця)");

  // Топологічне сортування (орієнтований ациклічний граф)
  cout << endl << "--- Тест 2: Топологічне сортування (DAG) ---" << endl;
  int M = 6;
  auto dag = list_create(M);
  // DAG: 5->2, 5->0, 4->0, 4->1, 2->3, 3->1
  list_add_edge(dag, 5, 2, 1, true);
  list_add_edge(dag, 5, 0, 1, true);
  list_add_edge(dag, 4, 0, 1, true);
  list_add_edge(dag, 4, 1, 1, true);
  list_add_edge(dag, 2, 3, 1, true);
  list_add_edge(dag, 3, 1, 1, true);
  list_print(dag, "DAG (список)");
  auto topo = list_kahn(dag);
  if (topo.empty())
    cout << "  Граф має цикл – топологічне сортування неможливе!" << endl;
  else {
    cout << "  Топологічний порядок (Kahn): ";
    for (int v : topo)
      cout << v << " ";
    cout << endl;
  }

  // Незв'язний граф
  cout << endl << "--- Тест 3: Незв'язний граф ---" << endl;
  auto m2 = mat_create(6);
  mat_add_edge(m2, 0, 1, 1.0, false);
  mat_add_edge(m2, 1, 2, 2.0, false);
  mat_add_edge(m2, 3, 4, 3.0, false);
  // вершина 5 ізольована
  cout << "  Матриця: граф "
       << (mat_is_connected(m2) ? "зв'язний" : "незв'язний") << endl;
  {
    vector<int> comp;
    int nc = mat_components(m2, comp);
    cout << "  Кількість компонент: " << nc << endl;
    for (int i = 0; i < 6; ++i)
      cout << "  Вершина " << i << " -> компонента " << comp[i] << endl;
  }

  // Вивід DOT-файлу
  cout << endl << "[Візуалізація]" << endl;
  export_dot(g, "graph_demo.dot", false, &mst, "Demo");
  export_dot(dag, "dag_demo.dot", true, nullptr, "DAG");
}

// ─────────────────────────────────────────────────────────────────────────────
//  ІНТЕРАКТИВНИЙ РЕЖИМ
// ─────────────────────────────────────────────────────────────────────────────

void print_menu() {
  cout << endl << "╔══════════════════════════════════════════════╗" << endl;
  cout << "║  МЕНЮ (інтерактивний режим)                  ║" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "║  Граф/структура даних:                       ║" << endl;
  cout << "║  1. Задати нову матрицю суміжності           ║" << endl;
  cout << "║  2. Задати новий список суміжності           ║" << endl;
  cout << "║  3. Додати ребро                             ║" << endl;
  cout << "║  4. Видалити ребро                           ║" << endl;
  cout << "║  5. Вивести граф                             ║" << endl;
  cout << "║  6. Генерувати випадковий граф               ║" << endl;
  cout << "║  7. Конвертувати подання                     ║" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "║  Алгоритми:                                  ║" << endl;
  cout << "║  8.  Перевірити зв'язність                   ║" << endl;
  cout << "║  9.  Знайти компоненти зв'язності            ║" << endl;
  cout << "║  10. DFS                                     ║" << endl;
  cout << "║  11. BFS                                     ║" << endl;
  cout << "║  12. Dijkstra                                ║" << endl;
  cout << "║  13. Топологічне сортування (Kahn)           ║" << endl;
  cout << "║  14. BFS кістякове дерево                    ║" << endl;
  cout << "║  15. MST Kruskal                             ║" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "║  16. Зберегти граф у DOT-файл                ║" << endl;
  cout << "║   0. Вийти                                   ║" << endl;
  cout << "╚══════════════════════════════════════════════╝" << endl;
  cout << "Оберіть: ";
}

void run_interactive() {
  cout << endl
       << "╔══════════════════════════════════════════════════════════════╗"
       << endl;
  cout << "║              ІНТЕРАКТИВНИЙ РЕЖИМ                             ║"
       << endl;
  cout << "╚══════════════════════════════════════════════════════════════╝"
       << endl;

  int n = 0;
  bool directed = false;
  bool use_matrix = true;
  vector<vector<double>> mat;
  AdjList lst;

  auto ensure_graph = [&]() -> bool {
    if (n == 0) {
      cout << "Спочатку задайте граф (пункти 1, 2 або 6)." << endl;
      return false;
    }
    return true;
  };
  // Синхронізація: якщо use_matrix – lst будується з mat, і навпаки
  auto sync = [&]() {
    if (use_matrix)
      lst = mat_to_list(mat);
    else
      mat = list_to_mat(lst);
  };

  int choice;
  while (true) {
    print_menu();
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }

    if (choice == 0)
      break;

    if (choice == 1) {
      cout << "Кількість вершин: ";
      cin >> n;
      cout << "Орієнтований? (1=так, 0=ні): ";
      cin >> directed;
      mat = mat_create(n);
      lst = list_create(n);
      use_matrix = true;
      cout << "Матриця суміжності створена (" << n << " вершин)." << endl;

    } else if (choice == 2) {
      cout << "Кількість вершин: ";
      cin >> n;
      cout << "Орієнтований? (1=так, 0=ні): ";
      cin >> directed;
      lst = list_create(n);
      mat = mat_create(n);
      use_matrix = false;
      cout << "Список суміжності створений (" << n << " вершин)." << endl;

    } else if (choice == 3) {
      if (!ensure_graph())
        continue;
      int u, v;
      double w;
      cout << "u v вага: ";
      cin >> u >> v >> w;
      if (u < 0 || v < 0 || u >= n || v >= n) {
        cout << "Некоректні вершини." << endl;
        continue;
      }
      if (use_matrix)
        mat_add_edge(mat, u, v, w, directed);
      else
        list_add_edge(lst, u, v, w, directed);
      sync();
      cout << "Ребро додано." << endl;

    } else if (choice == 4) {
      if (!ensure_graph())
        continue;
      int u, v;
      cout << "u v: ";
      cin >> u >> v;
      if (use_matrix)
        mat_remove_edge(mat, u, v, directed);
      else
        list_remove_edge(lst, u, v, directed);
      sync();
      cout << "Ребро видалено." << endl;

    } else if (choice == 5) {
      if (!ensure_graph())
        continue;
      mat_print(mat, "Матриця суміжності");
      list_print(lst, "Список суміжності");
      ascii_list_print(lst, "ASCII граф");

    } else if (choice == 6) {
      cout << "Кількість вершин: ";
      cin >> n;
      cout << "Кількість ребер: ";
      int e;
      cin >> e;
      cout << "Орієнтований? (1=так, 0=ні): ";
      cin >> directed;
      mat = mat_random(n, e, directed);
      lst = mat_to_list(mat);
      use_matrix = true;
      cout << "Випадковий граф згенеровано." << endl;
      mat_print(mat, "Матриця суміжності");
      ascii_list_print(lst, "Список суміжності");

    } else if (choice == 7) {
      if (!ensure_graph())
        continue;
      use_matrix = !use_matrix;
      sync();
      cout << "Активне подання: " << (use_matrix ? "матриця" : "список")
           << endl;

    } else if (choice == 8) {
      if (!ensure_graph())
        continue;
      cout << "  Матриця: "
           << (mat_is_connected(mat) ? "зв'язний" : "незв'язний") << endl;
      cout << "  Список:  "
           << (list_is_connected(lst) ? "зв'язний" : "незв'язний") << endl;

    } else if (choice == 9) {
      if (!ensure_graph())
        continue;
      vector<int> comp;
      int nc = list_components(lst, comp);
      cout << "  Кількість компонент: " << nc << endl;
      for (int i = 0; i < n; ++i)
        cout << "  Вершина " << i << " -> компонента " << comp[i] << endl;

    } else if (choice == 10) {
      if (!ensure_graph())
        continue;
      int start;
      cout << "Стартова вершина: ";
      cin >> start;
      print_order(mat_dfs(mat, start), "  DFS матриця (довільн.)");
      print_order(mat_dfs_sorted(mat, start), "  DFS матриця (за вагою)");
      print_order(list_dfs(lst, start), "  DFS список  (довільн.)");
      print_order(list_dfs_sorted(lst, start), "  DFS список  (за вагою)");

    } else if (choice == 11) {
      if (!ensure_graph())
        continue;
      int start;
      cout << "Стартова вершина: ";
      cin >> start;
      print_order(mat_bfs(mat, start), "  BFS матриця (довільн.)");
      print_order(mat_bfs_sorted(mat, start), "  BFS матриця (за вагою)");
      print_order(list_bfs(lst, start), "  BFS список  (довільн.)");
      print_order(list_bfs_sorted(lst, start), "  BFS список  (за вагою)");

    } else if (choice == 12) {
      if (!ensure_graph())
        continue;
      int src;
      cout << "Вершина-джерело: ";
      cin >> src;
      auto dr = list_dijkstra(lst, src);
      print_dijkstra(dr, src, n);

    } else if (choice == 13) {
      if (!ensure_graph())
        continue;
      auto topo = list_kahn(lst);
      if (topo.empty())
        cout << "  Є цикл – топологічне сортування неможливе." << endl;
      else {
        cout << "  Топологічний порядок: ";
        for (int v : topo)
          cout << v << " ";
        cout << endl;
      }

    } else if (choice == 14) {
      if (!ensure_graph())
        continue;
      int start;
      cout << "Стартова вершина: ";
      cin >> start;
      auto st = list_bfs_span_tree(lst, start);
      print_span_tree(st, "BFS Span Tree (список)");
      auto st_m = mat_bfs_span_tree(mat, start);
      print_span_tree(st_m, "BFS Span Tree (матриця)");

    } else if (choice == 15) {
      if (!ensure_graph())
        continue;
      auto mst = list_kruskal(lst);
      print_span_tree(mst, "MST Kruskal (список)");
      auto mst_m = mat_kruskal(mat);
      print_span_tree(mst_m, "MST Kruskal (матриця)");

    } else if (choice == 16) {
      if (!ensure_graph())
        continue;
      string fname;
      cout << "Ім'я файлу (.dot): ";
      cin >> fname;
      if (fname.length() < 4 || fname.substr(fname.length() - 4) != ".dot") {
        fname += ".dot";
      }
      export_dot(lst, fname, directed);
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────────────────────

int main() {
#ifdef _WIN32
  SetConsoleOutputCP(65001);
  SetConsoleCP(65001);
#endif

  cout << "╔══════════════════════════════════════════════════════════════╗"
       << endl;
  cout << "║   Лабораторна робота №6 – Алгоритми на графах                ║"
       << endl;
  cout << "║   Подання: Матриця суміжності + Список суміжності            ║"
       << endl;
  cout << "╚══════════════════════════════════════════════════════════════╝"
       << endl;

  cout << endl << "Оберіть режим:" << endl;
  cout << "  1. Інтерактивний" << endl;
  cout << "  2. Демонстраційний" << endl;
  cout << "  3. Benchmark" << endl;
  cout << "  4. Всі режими послідовно" << endl;
  cout << "Ваш вибір: ";

  int mode;
  cin >> mode;
  switch (mode) {
  case 1:
    run_interactive();
    break;
  case 2:
    run_demo();
    break;
  case 3:
    run_benchmark();
    break;
  case 4:
    run_demo();
    run_benchmark();
    run_interactive();
    break;
  default:
    cout << "Невідомий режим." << endl;
  }

  cout << endl << "=== Програма завершена ===" << endl;
  return 0;
}