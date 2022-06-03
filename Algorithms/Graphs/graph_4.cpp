#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> g;
std::vector<bool> used;
std::vector<bool> color;

bool dfs(int v, bool c = 0) {
    used[v] = true;
    color[v] = c;
    for (int u: g[v]) {
        if (!used[u]) {
            dfs(u, 1 - c);
        } else if (color[u] == c) {
            return false;
        }
    }
    return true;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    used = std::vector<bool>(n, false);
    g = std::vector<std::vector<int>>(n, std::vector<int>(0));
    color = std::vector<bool>(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    for (int v = 0; v < n; ++v) {
        if (!used[v] && !dfs(v)) {
            std::cout << "NO\n";
            return 0;
        }
    }
    std::cout << "YES\n";
    std::vector<int> ans(0);
    for (int v = 0; v < n; ++v)
        if (!color[v])
            ans.push_back(v);
    std::sort(ans.begin(), ans.end());
    for (int v: ans)
        std::cout << v << ' ';
    std::cout << '\n';
}

/*
 * 1 - 2 - 3
 *     4
 *     |
 *     5-0
 *
 *
 *     1 - 2
 *      \ /
 *       0
 * */