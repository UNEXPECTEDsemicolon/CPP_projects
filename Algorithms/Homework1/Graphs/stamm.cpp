#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

std::vector<std::vector<int>> g;
std::vector<int> color;

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

int main() {
    int n, m;
    std::cin >> n >> m;
    color = std::vector<int>(n, 0);
    g = std::vector<std::vector<int>>(n, std::vector<int>(0));
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        g[--x].push_back(--y);
        g[y].push_back(x);
    }
    auto KSSs = std::vector<std::unordered_set<int>>();
    for (int v = 0; v < n; ++v) {
        if (color[v] == 0) {
            KSSs.push_back(std::unordered_set<int>());
            dfs_kss(v, *(KSSs.end()-1));
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
    std::cout << KSSs.size() << '\n';
    for (auto c: classes)
        std::cout << c + 1 << ' ';
    std::cout << '\n';
}
