#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <string>

std::vector<std::vector<int>> g;
std::vector<int> color;
std::vector<int> parents;
size_t ans = 0;

void dfs(int v, int p = -1) {
    color[v] = 1;
    parents[v] = p;
    for (int u: g[v]) {
        if (color[u] == 0) {
            dfs(u, v);
            color[u] = 2;
        } else if (u != p && color[u] == 1) {
            bool OK = true;
            auto cycle = std::unordered_set<int>();
            cycle.insert(u);
            for (auto w = v; w != u; w = parents[w]) {
                cycle.insert(w);
            }
            for (auto w1: cycle) {
                int neighs = 0;
                for (auto w: g[w1]) {
                    if (cycle.find(w) != cycle.end())
                        ++neighs;
                    if (neighs > 2) {
                        OK = false;
                        break;
                    }
                }
                if (!OK)
                    break;
            }
            if (OK)
                ans = std::max(cycle.size(), ans);
        }
    }
}

int main() {
    int m;
    auto names = std::unordered_map<std::string, int>();
    std::cin >> m;
    int id = 0;
    auto qs = std::vector<std::pair<int, int>>(m);
    for (int i = 0; i < m; ++i) {
        std::string x, y;
        std::cin >> x >> y;
        if (names.find(x) == names.end())
            names[x] = id++;
        if (names.find(y) == names.end())
            names[y] = id++;
        qs[i] = {names[x], names[y]};
    }
    auto n = int(names.size());
    g = std::vector<std::vector<int>>(n, std::vector<int>(0));
    color = std::vector<int>(n);
    parents = std::vector<int>(n);
    for (int i = 0; i < m; ++i) {
        auto x = qs[i].first;
        auto y = qs[i].second;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    for (int v = 0; v < n; ++v) {
        if (color[v] == 0) {
            dfs(v);
        }
    }
    std::cout << ans << '\n';
}
