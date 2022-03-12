#include <iostream>

const size_t INF = 100000000;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t n;
    std::cin >> n;
    auto x = new int [n];
    for (size_t i = 0; i < n; ++i)
        std::cin >> x[i];
    int s;
    std::cin >> s;
    auto dp = new size_t [s + 1];
    dp[0] = 0;
    for (int i = 1; i <= s; ++i) {
        dp[i] = INF;
        for (size_t j = 0; j < n; ++j) {
            if (i - x[j] >= 0)
                dp[i] = std::min(dp[i], dp[i - x[j]] + 1);
        }
    }
    size_t m = dp[s];
    if (m >= INF / 10) {
        std::cout << -1 << '\n';
    } else {
        auto ans = new size_t[m];
        int cur_sum = s;
        for (size_t i = 0; i < m; ++i) {
            size_t min_cnt = INF;
            for (size_t j = 0; j < n; ++j) {
                if (cur_sum - x[j] >= 0 && dp[cur_sum - x[j]] < min_cnt) {
                    min_cnt = dp[cur_sum - x[j]];
                    ans[i] = j;
                }
            }
            cur_sum -= x[ans[i]];
        }
        std::cout << m << '\n';
        for (size_t i = 0; i < m; ++i) {
            std::cout << x[ans[i]] << ' ';
        }
        std::cout << '\n';
        delete[] ans;
    }
    delete[] x;
    delete[] dp;
}