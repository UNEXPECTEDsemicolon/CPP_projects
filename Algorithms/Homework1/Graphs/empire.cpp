//#include <iostream>
#include <stdio.h>
#include <vector>

std::vector<std::vector<int>> g;
std::vector<std::vector<bool>> used;
std::vector<int> ans;

bool read_int_unlocked(int & out) {
    int c = _getchar_nolock();
    int x = 0;
    int neg = 0;

    for (; !('0'<=c && c<='9') && c != '-'; c = _getchar_nolock()) {
        if (c == EOF)
            return false;
    }
    if (c == '-') {
        neg = 1;
        c = _getchar_nolock();
    }
    if (c == EOF)
        return false;
    for (; '0' <= c && c <= '9' ; c = _getchar_nolock())
        x = x*10 + c - '0';
    out = neg ? -x : x;
    return true;
}

void euler(int v) {
    for (auto u: g[v]) {
        if (!used[v][u]) {
            used[v][u] = true;
            euler(u);
        }
    }
    ans.push_back(v);
}

int main() {
    int n, a;
    scanf("%d %d", &n, &a);
//    std::cin >> n >> a;
    g.assign(n, std::vector<int>());
    used.assign(n, std::vector<bool>(n, false));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int x;
            read_int_unlocked(x);
//            std::cin >> x;
            if (!x && i != j)
                g[i].push_back(j);
        }
    }
    euler(--a);
    for (size_t i = ans.size() - 1; i >= 1; --i)
        printf("%d %d\n", ans[i] + 1, ans[i - 1] + 1);
}