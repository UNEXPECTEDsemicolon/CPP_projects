#include <iostream>
#include <vector>
#include <algorithm>

//std::vector<std::vector<int>> g;
//std::vector<bool> used;
//std::vector<int> tin, ret;
//std::vector<bool> ans;

void dfs(int v, std::vector<bool>& used, std::vector<std::vector<int>>& g,
         std::vector<int>& tin, std::vector<int>& ret,/* std::vector<bool>& ans,*/ int& timer, int p = -1) {
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
            dfs(u, used, g, tin, ret,/* ans,*/ timer, v);
            ret[v] = std::min(ret[v], ret[u]);
//            if (p != -1 && ret[u] >= tin[v]) {
//                ans[v] = true;
//            }
        }
    }
//    if (p == -1 && count >= 2)
//        ans[v] = true;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    auto used = std::vector<bool>(n, false);
    auto g = std::vector<std::vector<int>>(n, std::vector<int>());
    auto tin = std::vector<int>(n);
    auto ret = std::vector<int>(n);
//    auto ans = std::vector<bool>(n, false);
    int timer = 0;
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        g[--x].push_back(--y);
        g[y].push_back(x);
    }
    for (int v = 0; v < n; ++v)
        if (!used[v])
            dfs(v, used, g, tin, ret,/* ans,*/ timer);
//    auto final_ans = std::vector<int>();
//    for (int i = 0; i < n; ++i) {
//        if (ans[i])
//            final_ans.push_back(i);
//    }
//    std::cout << final_ans.size() << '\n';
//    for (auto a__: final_ans)
//        std::cout << a__ + 1 << ' ';
//    std::cout << '\n';
    std::cout << "0\n";
}
