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

void meta_dfs(int v, std::vector<std::unordered_set<int>>& g, std::vector<int>& color, std::vector<int>& tout) {
    for (int u: g[v]) {
        if (color[u] == 0) {
            color[u] = 1;
            meta_dfs(u, g, color, tout);
            color[u] = 2;
        }
    }
    tout[v] = timer++;
}

struct Comp {
    std::vector<int> tout = ::tout;
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
    auto meta_color = std::vector<int>(new_n, 0);
    auto meta_tout = std::vector<int>(new_n);
    timer = 0;
    for (int v = 0; v < new_n; ++v)
        if (meta_color[v] == 0)
            meta_dfs(v, meta_edges, meta_color, meta_tout);
    auto meta_p = std::vector<int>(new_n);
    for (int i = 0; i < new_n; ++i)
        meta_p[i] = i;
    std::sort(meta_p.begin(), meta_p.end(), Comp{meta_tout});
    auto ans = std::vector<std::pair<int, int>>();
    auto babies = std::vector<int>();
    for (auto v: meta_p) {
        if (meta_edges_inv[v].size() == 0) {
            for (auto baby: babies) {
                ans.push_back({baby, v});
                meta_edges[baby].insert(v);
                meta_edges_inv[v].insert(baby);
            }
            babies.clear();
        }
        if (meta_edges[v].size() == 0) {
            babies.push_back(v);
        }
    } for (auto baby: babies) {
        ans.push_back({baby, meta_p[0]});
        meta_edges[baby].insert(meta_p[0]);
        meta_edges_inv[meta_p[0]].insert(baby);
    } babies.clear();
    for (int i = 0; i < new_n; ++i) {
        if (meta_edges[meta_p[i]].size() == 0) {
            ans.push_back({meta_p[i-1], meta_p[i]});
            meta_edges[meta_p[i-1]].insert(meta_p[i]);
            meta_edges_inv[meta_p[i]].insert(meta_p[i-1]);
        }
    }
    std::cout << ans.size() << '\n';
    for (auto edge: ans) {
        std::cout << *((KSSs.begin() + edge.first)->begin()) + 1 << ' '
        << *((KSSs.begin() + edge.second)->begin()) + 1 << '\n';
    }
}