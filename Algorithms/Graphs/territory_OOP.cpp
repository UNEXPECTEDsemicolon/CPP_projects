#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

template <typename T = std::vector<int>>
        struct Graph {
    int n;
    std::vector<T> g;
    std::vector<int> color;
    std::vector<int> tin, tout;
    int timer = 0;
    
    Graph(int n)
    : n(n), g(std::vector<T>(n, T())),
    color(std::vector<int>(n, 0)),
    tin(std::vector<int>(n)), tout(std::vector<int>(n)) {}
          
    Graph(int n, std::vector<T>&& g)
    : n(n), g(g), color(std::vector<int>(n, 0)),
    tin(std::vector<int>(n)), tout(std::vector<int>(n)) {}

    void dfs_kss(int v, std::unordered_set<int>& KSS) {
        color[v] = 1;
        KSS.insert(v);
        for (int u: g[v]) {
            if (color[u] == 0) {
                dfs_kss(u, KSS);
            }
        }
        color[v] = 2;
    }

    void dfs(int v) {
        tin[v] = timer++;
        for (int u: g[v]) {
            if (color[u] == 0) {
                color[u] = 1;
                dfs(u);
                color[u] = 2;
            }
        }
        tout[v] = timer++;
    }

    struct Comp {
        Graph* graph;
        bool operator()(const int& left, const int& right) {
            return graph->tout[left] > graph->tout[right];
        }
    };

    std::vector<int> topsort() {
        for (int v = 0; v < n; ++v)
            if (color[v] == 0)
                dfs(v);
            auto p = std::vector<int>(n);
            for (int i = 0; i < n; ++i)
                p[i] = i;
            std::sort(p.begin(), p.end(), Comp{this});
            return p;
    }
};

int main() {
    int n, m;
    std::cin >> n >> m;
    auto graph = Graph<std::vector<int>>(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        graph.g[--x].push_back(--y);
    }
    auto p = graph.topsort();
    //    for (auto pp: p)
    //        std::cout << pp << ' ';
    //    std::cout << '\n';
    auto inv_graph = Graph<std::vector<int>>(n);
    for (int v = 0; v < n; ++v)
        for (auto u: graph.g[v])
            inv_graph.g[u].push_back(v);
        auto KSSs = std::vector<std::unordered_set<int>>();
        for (auto v: p) {
            if (inv_graph.color[v] == 0) {
                KSSs.push_back(std::unordered_set<int>());
                inv_graph.dfs_kss(v, *(KSSs.end()-1));
            }
        }
        int new_n = KSSs.size();
        if (new_n == 1) {
            std::cout << "0\n";
            return 0;
        }
        auto classes = std::vector<int>(n);
        int i = 0;
        for (auto KSS: KSSs) {
            for (auto v: KSS) {
                classes[v] = i;
            }
            ++i;
        }
        //    auto meta_graph = Graph<std::unordered_set<int>>()
        auto meta_edges = std::vector<std::unordered_set<int>>(new_n, std::unordered_set<int>());
        auto meta_edges_inv = std::vector<std::unordered_set<int>>(new_n, std::unordered_set<int>());
        for (int v = 0; v < n; ++v) {
            for (auto u: g[v]) {
                if (classes[v] != classes[u]) {
                    meta_edges[classes[v]].insert(classes[u]);
                    meta_edges_inv[classes[u]].insert(classes[v]);
                }
            }
        }
        auto ancestors = std::vector<int>();
        auto babies = std::vector<int>();
        for (int v = 0; v < new_n; ++v) {
            if (meta_edges[v].size() == 0)
                babies.push_back(v);
            if (meta_edges_inv[v].size() == 0)
                ancestors.push_back(v);
        }
        std::cout << babies.size() * ancestors.size() << '\n';
        for (size_t i = 0; i < babies.size(); ++i) {
            for (size_t j = 0; j < ancestors.size(); ++j) {
                std::cout << *((KSSs.begin() + babies[i])->begin()) + 1 << ' '
                << *((KSSs.begin() + ancestors[j])->begin()) + 1 << '\n';
            }
        }
}