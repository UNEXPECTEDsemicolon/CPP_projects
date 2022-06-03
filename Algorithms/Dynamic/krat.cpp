#include <stdio.h>
#include <algorithm>

int main() {
    size_t n;
    scanf("%zu", &n);
    auto *a = new long long [n];
    for (size_t i = 0; i < n; ++i)
        scanf("%lld", &a[i]);
    auto *dp = new size_t [n];
    for (size_t i = 0; i < n; ++i)
        dp[i] = 1;
    dp[n - 1] = 1;
    for (size_t i = n - 2; i + 1 >= 1; --i)
        for (size_t j = i + 1; j < n; ++j)
            if (a[j] % a[i] == 0)
                dp[i] = std::max(dp[i], dp[j] + 1);
    auto ans = dp[0];
    for (size_t i = 0; i < n; ++i)
        ans = std::max(ans, dp[i]);
    printf("%zu\n", ans);
    delete [] dp;
    delete [] a;
}