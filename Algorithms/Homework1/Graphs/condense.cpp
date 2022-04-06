#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

std::vector<std::vector<int>> g;
std::vector<std::vector<int>> g2;
std::vector<int> color;
std::vector<int> tin, tout;
int timer = 0;

void dfs_kss(int v, const std::vector<std::vector<int>>& g, std::unordered_set<int>& KSS) {
    color[v] = 1;
    KSS.insert(v);
    for (int u: g[v]) {
        if (color[u] == 0) {
            dfs_kss(u, g, KSS);
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
    bool operator()(const int& left, const int& right) {
        return tout[left] > tout[right];
    }
};

int main() {
    int n, m;
    std::cin >> n >> m;
    color = std::vector<int>(n, 0);
    g = std::vector<std::vector<int>>(n, std::vector<int>(0));
    tin = std::vector<int>(n);
    tout = std::vector<int>(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        g[--x].push_back(--y);
    }
    for (int v = 0; v < n; ++v)
        if (color[v] == 0)
            dfs(v);
    auto p = std::vector<int>(n);
    for (int i = 0; i < n; ++i)
        p[i] = i;
    std::sort(p.begin(), p.end(), Comp());
    color.assign(n, 0);
    g2 = std::vector<std::vector<int>>(n, std::vector<int>(0));
    for (int v = 0; v < n; ++v)
        for (auto u: g[v])
            g2[u].push_back(v);
    auto KSSs = std::vector<std::unordered_set<int>>();
    for (auto v: p) {
        if (color[v] == 0) {
            KSSs.push_back(std::unordered_set<int>());
            dfs_kss(v, g2, *(KSSs.end()-1));
        }
    }
    auto classes = std::vector<int>(n);
    int i = 0;
    for (auto KSS: KSSs) {
        for (auto v: KSS) {
            classes[v] = i;
        }
        ++i;
    }
    auto edges = std::vector<std::unordered_set<int>>(classes.size(), std::unordered_set<int>());
    for (int v = 0; v < n; ++v)
        for (auto u: g[v])
            if (classes[v] != classes[u])
                edges[classes[v]].insert(classes[u]);
    int ans = 0;
    for (auto v: edges)
        ans += v.size();
    std::cout << ans << '\n';
}
