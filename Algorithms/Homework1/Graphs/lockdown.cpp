#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<std::pair<int, int>>> g;
std::vector<bool> used;
std::vector<int> tin, ret, ans;
int timer = 0;

void dfs(int v, int p = -1) {
    tin[v] = timer++;
    ret[v] = tin[v];
    used[v] = true;
    for (auto u: g[v]) {
        if (u.first == p) continue;
        if (used[u.first]) {
            ret[v] = std::min(ret[v], tin[u.first]);
        } else {
            dfs(u.first, v);
            ret[v] = std::min(ret[v], ret[u.first]);
            if (ret[u.first] == tin[u.first]) {
                ans.push_back(u.second);
            }
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    used = std::vector<bool>(n, false);
    g = std::vector<std::vector<std::pair<int, int>>>(n, std::vector<std::pair<int, int>>());
    tin = std::vector<int>(n);
    ret = std::vector<int>(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        g[--x].push_back({--y, i});
        g[y].push_back({x, i});
    }
    for (int v = 0; v < n; ++v)
        if (!used[v])
            dfs(v);
    std::sort(ans.begin(), ans.end());
    std::cout << ans.size() << '\n';
    for (auto a: ans)
        std::cout << a + 1 << ' ';
    std::cout << '\n';
}
