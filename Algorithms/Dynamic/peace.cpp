#include <iostream>

int main() {
    size_t n;
    std::cin >> n;
    ++n;
    auto dp = new long long *[n];
    auto dps = new long long *[n];
    for (size_t i = 0; i < n; ++i) {
        dp[i] = new long long[n];
        dps[i] = new long long[n];
    }
    for (size_t j = 0; j < n; ++j) dps[0][j] = 1;
    for (size_t i = 1; i < n; ++i) {
        dps[i][0] = 0;
        for (size_t j = 1; j < n; ++j) {
            dp[i][j] = (i >= j ? dps[i - j][j / 2] : 0);
            dps[i][j] = dps[i][j - 1] + dp[i][j];
        }
    }
    long long ans = 0;
    for (size_t j = 1; j < n; ++j)
        ans += dp[n-1][j];
    std::cout << ans << '\n';
    for (size_t i = 0; i < n; ++i) {
        delete [] dp[i];
        delete [] dps[i];
    }
    delete [] dp;
    delete [] dps;
}