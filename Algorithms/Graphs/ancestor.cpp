#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> g;
std::vector<bool> used;
std::vector<int> tin, tout;
int timer = 0;

void dfs(int v) {
    used[v] = true;
    for (int u: g[v]) {
        if (!used[u]) {
            tin[u] = timer++;
            dfs(u);
            tout[u] = timer++;
        }
    }
}

int main() {
    int n;
    std::cin >> n;
    ++n;
    used = std::vector<bool>(n, false);
    g = std::vector<std::vector<int>>(n, std::vector<int>(0));
    tin = std::vector<int>(n);
    tout = std::vector<int>(n);
    for (int i = 1; i < n; ++i) {
        int x;
        std::cin >> x;
        g[x].push_back(i);
    }
    dfs(0);
    int m;
    std::cin >> m;
    for (int i  =0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        std::cout << bool(tin[a] < tin[b] && tout[b] < tout[a]) << '\n';
    }
}
