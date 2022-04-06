#include <iostream>
#include <vector>
#include <algorithm>
 
std::vector<std::vector<int>> g;
std::vector<bool> used;
std::vector<int> tin, ret, ans;
int timer = 0;
 
void dfs(int v, int p = -1) {
    tin[v] = timer++;
    ret[v] = tin[v];
    used[v] = true;
    int count = 0;
    for (auto u: g[v]) {
        if (u == p) continue;
        if (used[u]) {
            ret[v] = std::min(ret[v], tin[u]);
        } else {
            ++count;
            dfs(u, v);
            ret[v] = std::min(ret[v], ret[u]);
            if (p != -1 && ret[u] >= tin[v]) {
                ans.push_back(v);
            }
        }
    }
    if (p == -1 && count >= 2)
        ans.push_back(v);
}
 
int main() {
    int n, m;
    std::cin >> n >> m;
    used = std::vector<bool>(n, false);
    g = std::vector<std::vector<int>>(n, std::vector<int>());
    tin = std::vector<int>(n);
    ret = std::vector<int>(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        g[--x].push_back(--y);
        g[y].push_back(x);
    }
    for (int v = 0; v < n; ++v)
        if (!used[v])
            dfs(v);
        std::sort(ans.begin(), ans.end());
        auto last = std::unique(ans.begin(), ans.end());
        ans.erase(last, ans.end());
        std::cout << ans.size() << '\n';
        for (auto a: ans)
            std::cout << a + 1 << ' ';
        std::cout << '\n';
}