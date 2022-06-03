#include <iostream>
#include <stdio.h>

const int INF = (1ull<<31) - 1;

struct MyPair {
    int first;
    int second;

    MyPair(): first(0), second(INF) {}

    MyPair(int first, int second): first(first), second(second) {}

    bool operator<(const MyPair& right) const {
        if (second < right.second)
            return true;
        if (second > right.second)
            return false;
        return first < right.first;
    }
};

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

//void write_int_unlocked(int x) {
//    while (x) {
//        putchar_unlocked(x % 10);
//        x /= 10;
//    }
//}

int main() {
    int n;
    scanf("%d", &n);
//    std::cin >> n;
    auto a = new int [n];
    for (int i = 0; i < n; ++i) {
        read_int_unlocked(a[i]);
    }
    auto dp = new MyPair [n];
    auto way = new int [n];
    for (int i = 0; i < n; ++i) {
        way[i] = -1;
    }
    int best_k = 0;
    int best_end = -1;
    for (int i = 0; i < n; ++i) {
        int l = 0, r = n;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (dp[m].second < -a[i])
                l = m;
            else if (dp[m].second > -a[i])
                r = m;
            else
                l = r = m;
        }
        int k = (dp[l].second >= -a[i] ? l : r);
        if (dp[k].second == INF) {
            ++best_k;
            best_end = i;
        }
        if (k != 0)
            way[i] = dp[k - 1].first;
        dp[k] = {i, -a[i]};
    }
    auto ans = new int [best_k];
    for (int i = best_end, k = best_k - 1; i >= 0 && k >= 0; i = way[i], --k)
        ans[k] = i + 1;
    printf("%d\n", best_k);
    for (int i = 0; i < best_k; ++i)
        printf("%d ", ans[i]);
    printf("\n");
    delete [] a;
    delete [] dp;
    delete [] way;
    delete [] ans;
}