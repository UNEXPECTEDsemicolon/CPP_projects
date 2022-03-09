#include <stdio.h>
#include <algorithm>

size_t solve(size_t i, long long j, size_t **dp, long long *x, long long s) {
    if (dp[i][j] != 102)
        return dp[i][j];
    if (j <= s && j < x[i - 1])
        return dp[i][j] = solve(i - 1, j, dp, x, s);
    return dp[i][j] = std::min(solve(i - 1, j, dp, x, s), solve(i, j - x[i - 1], dp, x, s) + 1);
}

int main() {
    size_t n;
    scanf("%zu", &n);
    auto *x = new long long [n];
    for (size_t i = 0; i < n; ++i)
        scanf("%lld", &x[i]);
    long long s;
    scanf("%lld", &s);
    auto **dp = new size_t * [n + 1];
    for (size_t i = 0; i <= n; ++i)
        dp[i] = new size_t [s + 1];
    for (long long i = 0; i <= s; ++i)
        dp[0][i] = 101;
    dp[0][0] = 0;
    for (size_t i = 1; i <= n; ++i) {
        for (long long j = 0; j <= s; ++j)
            dp[i][j] = 102;
    }
    size_t k = solve(n, s, dp, x, s);
    if (k == 101)
        printf("-1\n");
    else {
        auto *ans = new long long[k];
        long long cur_s = s;
        long long j = s;
        for (size_t i = n, z = 0; z < k; ) {
            if (z == k - 1) {
                ans[z] = j;
                break;
            }
            if (j < x[i-1] || dp[i - 1][j] < dp[i][j - x[i - 1]] + 1) {
                --i;
            } else {
                j -= x[i - 1];
                ans[z++] = (cur_s - j);
                cur_s = j;
            }
        }
        printf("%zu\n", k);
        for (size_t z = 0; z < k; ++z)
            printf("%lld ", ans[z]);
        printf("\n");
        delete [] ans;
    }
    for (size_t i = 0; i <= n; ++i) {
        delete[] dp[i];
    }
    delete [] dp;
    delete [] x;
}
