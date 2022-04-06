#include <iostream>
#include <vector>
#include <set>

const int INF = 1000000000;

struct Comp {

    bool operator()(const std::pair<int ,int>& p1, const std::pair<int, int>& p2) const {
        return p1.second != p2.second ? p1.second < p2.second : p1.first < p2.first;
    }
};

int main() {
    int n, m;
    std::cin >> n >> m;
    auto cost = std::vector<std::vector<int>>(n, std::vector<int>(n));
    auto g = std::vector<std::vector<int>>(n, std::vector<int>(0));
    for (int i  =0; i < m; ++i) {
        int u, v, x;
        std::cin >> u >> v >> x;
        cost[u][v] = cost[v][u] = x;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    int s;
    std::cin >> s;
    auto d = std::vector<int>(n, INF);
    auto used = std::vector<bool>(n, false);
    auto heap = std::set<std::pair<int, int>, Comp>();
    heap.insert({s, d[s]});
    d[s] = 0;
    for (int i = 0; i < n; ++i) {
        auto v_it = heap.begin();
        heap.erase(v_it);
        auto v = v_it->first;
        if (d[v] == INF)
            break;
        used[v] = true;
        for (auto u: g[v]) {
            if (d[v] + cost[v][u] < d[u]) {
                heap.erase({u, d[u]});
                heap.insert({u, d[u] = d[v] + cost[v][u]});
            }
        }
    }
    for (auto di: d) {
        std::cout << di << ' ';
    }
    std::cout << '\n';
}
