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

int main() {
//    std::ios_base::sync_with_stdio(0);
//    std::cin.tie(NULL);
    int n;
    scanf("%d", &n);
//    std::cin >> n;
    auto a = new int [n];
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
//        if (i % 10 == 0)
//            printf("i=%d\n", i);
//        std::cin >> a[i];
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
//        auto k_it = std::lower_bound(dp, dp + n, MyPair(i, -a[i]));
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
//    std::cout << best_k << '\n';
    printf("%d\n", best_k);
    for (int i = 0; i < best_k; ++i)
        printf("%d ", ans[i]);
//        std::cout << ans[i] << ' ';
    printf("\n");
//    std::cout << '\n';
    delete [] a;
    delete [] dp;
    delete [] way;
    delete [] ans;
}