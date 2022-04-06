//#include <iostream>
#include <stdio.h>
#include <vector>

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

void euler(int v, int**& g, int*& sizes, bool**& used, std::vector<int>& ans) {
    for (size_t i = 0; i < sizes[v]; ++i) {
        if (!used[v][g[v][i]]) {
            used[v][g[v][i]] = true;
            euler(g[v][i], g, sizes, used, ans);
        }
    }
    ans.push_back(v);
}

int main() {
    int n, a;
    scanf("%d %d", &n, &a);
//    std::cin >> n >> a;
    auto g = new int* [n];
    auto sizes = new int [n];
    auto used = new bool* [n];
    for (int i = 0; i < n; ++i) {
        g[i] = new int[n];
        sizes[i] = 0;
        used[i] = new bool [n];
        for (int j = 0; j < n; ++j)
            used[i][j] = false;
    }
    auto ans = std::vector<int>();
    size_t m = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int x;
            read_int_unlocked(x);
//            std::cin >> x;
            if (!x && i != j) {
                g[i][sizes[i]++] = j;
                ++m;
            }
        }
    }
    ans.reserve(m);
    euler(--a, g, sizes, used, ans);
    if (ans.size())
        for (size_t i = ans.size() - 1; i >= 1; --i)
            printf("%d %d\n", ans[i] + 1, ans[i - 1] + 1);
    for (int i = 0; i < n; ++i) {
        delete[] g[i];
        delete [] used[i];
    }
    delete [] g;
    delete [] sizes;
    delete [] used;
}