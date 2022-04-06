#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> g;
std::vector<int> color;
std::vector<int> parents;

void dfs(int v, std::vector<int>& cycle) {
    for (int u: g[v]) {
        if (color[u] == 0) {
            color[u] = 1;
            parents[u] = v;
            dfs(u, cycle);
            color[u] = 2;
            if (!cycle.empty())
                return;
        } else if (color[u] == 1) {
            cycle.push_back(u);
            for (auto w = v; w != u; w = parents[w]) {
                cycle.push_back(w);
            }
            return;
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    g = std::vector<std::vector<int>>(n, std::vector<int>());
    color = std::vector<int>(n, 0);
    parents = std::vector<int>(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        g[--x].push_back(--y);
    }
    auto res = std::vector<int>();
    for (int v = 0; v < n; ++v) {
        if (color[v] == 0) {
            dfs(v, res);
            if (!res.empty()) {
                std::cout << "YES\n";
                for (auto it = res.rbegin(); it != res.rend(); ++it)
                    std::cout << (*it + 1) << ' ';
                std::cout << '\n';
                return 0;
            }
        }
    }
    std::cout << "NO\n";
}
