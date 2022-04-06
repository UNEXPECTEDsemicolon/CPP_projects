#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> g;
std::vector<int> color;
std::vector<int> tin, tout;
int timer = 0;

bool dfs(int v) {
    tin[v] = timer++;
    for (int u: g[v]) {
        if (color[u] == 0) {
            color[u] = 1;
            if (!dfs(u))
                return false;
            color[u] = 2;
        } else if (color[u] == 1)
            return false;
    }
    tout[v] = timer++;
    return true;
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
    for (int v = 0; v < n; ++v) {
        if (color[v] == 0 && !dfs(v)) {
            std::cout << -1;
            return 0;
        }
    }
    auto ans = std::vector<int>(n);
    for (int i = 0; i < n; ++i)
        ans[i] = i;
    std::sort(ans.begin(), ans.end(), Comp());
    for (auto v: ans)
        std::cout << v + 1 << ' ';
    std::cout << '\n';
}
