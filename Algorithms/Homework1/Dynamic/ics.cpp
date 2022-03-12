#include <iostream>

int main() {
    size_t n, m;
    std::cin >> n;
    auto a = new int [n];
    for (size_t i = 0; i < n; ++i)
        std::cin >> a[i];
    std::cin >> m;
    auto b = new int [m];
    for (size_t i = 0; i < m; ++i)
        std::cin >> b[i];
    auto dp = new size_t* [n+1];
    for (size_t i = 0; i <= n; ++i)
        dp[i] = new size_t [m+1];
    for (size_t j = 0; j <= m; ++j)
        dp[0][j] = 0;
    for (size_t i = 1; i <= n; ++i) {
        dp[i][0] = 0;
        for (size_t j = 1; j <= m; ++j)
            dp[i][j] = std::max(std::max(dp[i - 1][j], dp[i][j - 1]), (a[i - 1] == b[j - 1]) * (dp[i - 1][j - 1] + 1));
    }
    std::cout << dp[n][m] << '\n';
    delete [] a;
    delete [] b;
    for (size_t i = 0; i <= n ; ++i)
        delete [] dp[i];
    delete [] dp;
}