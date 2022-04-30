#include <iostream>
#include <vector>
#include <algorithm>

const int INF = 100000000;
std::vector<std::vector<int>> g;
std::vector<std::vector<int>> cost;
std::vector<int> color;
std::vector<int> parents;

int ans = INF;

void dfs(int v, int p = -1) {
    for (int u: g[v]) {
        if (color[u] == 0) {
            color[u] = 1;
            dfs(u, v);
            color[u] = 2;
        } else if (u != p && color[u] == 1) {
            int new_len = cost[u][v];
            for (auto w = v; w != u; w = parents[w]) {
                new_len += cost[w][parents[w]];
            }
            ans = std::min(ans, new_len);
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    g.assign(n, std::vector<int>());
    auto& g_ = g;
    cost.assign(n, std::vector<int>(n, INF));
    color.assign(n, 0);
    parents.assign(n, -1);
    for (int i = 0; i < m; ++i) {
        int x, y, w;
        std::cin >> x >> y >> w;
        g[--x].push_back(--y);
        cost[x][y] = cost[y][x] = w;
    }
    for (int v = 0; v < n; ++v) {
        if (color[v] == 0) {
            dfs(v);
        }
    }
    std::cout << ans << '\n';
}
