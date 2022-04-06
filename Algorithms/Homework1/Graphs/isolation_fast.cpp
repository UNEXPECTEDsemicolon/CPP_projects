#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>

bool read_int_unlocked(int & out) {
    int c = getchar_unlocked();
    int x = 0;
    int neg = 0;

    for (; !('0'<=c && c<='9') && c != '-'; c = getchar_unlocked()) {
        if (c == EOF)
            return false;
    }
    if (c == '-') {
        neg = 1;
        c = getchar_unlocked();
    }
    if (c == EOF)
        return false;
    for (; '0' <= c && c <= '9' ; c = getchar_unlocked())
        x = x*10 + c - '0';
    out = neg ? -x : x;
    return true;
}

void dfs(int v, std::vector<bool>& used, std::vector<std::vector<int>>& g,
         std::vector<int>& tin, std::vector<int>& ret, std::vector<bool>& ans, int& timer, int p = -1) {
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
            dfs(u, used, g, tin, ret, ans, timer, v);
            ret[v] = std::min(ret[v], ret[u]);
            if (p != -1 && ret[u] >= tin[v]) {
                ans[v] = true;
            }
        }
    }
    if (p == -1 && count >= 2)
        ans[v] = true;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
//    std::cin >> n >> m;
    if (m == n * (n - 1) / 2) {
        printf("0\n");
        return 0;
    }
    auto used = std::vector<bool>(n, false);
    auto g = std::vector<std::vector<int>>(n, std::vector<int>());
    auto tin = std::vector<int>(n);
    auto ret = std::vector<int>(n);
    auto ans = std::vector<bool>(n, false);
    int timer = 0;
    for (int i = 0; i < m; ++i) {
        int x, y;
        read_int_unlocked(x);
        read_int_unlocked(y);
//        std::cin >> x >> y;
        g[--x].push_back(--y);
        g[y].push_back(x);
    }
//    for (int v = 0; v < n; ++v)
//        if (!used[v])
//            dfs(v, used, g, tin, ret, ans, timer);
    auto final_ans = std::vector<int>();
    for (int i = 0; i < n; ++i) {
        if (ans[i])
            final_ans.push_back(i);
    }
    std::cout << final_ans.size() << '\n';
    for (auto a: final_ans)
        printf("%d ", a + 1);
//        std::cout << a + 1 << ' ';
//    std::cout << '\n';
    printf("\n");
}
