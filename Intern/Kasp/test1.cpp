#include <iostream>
#include <vector>

static const long long INF = 1ul << 31;

int main() {
    size_t n;
    std::cin >> n;
    std::vector<long long> a(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> a[i];
        a[i] *= -1;
    }
    std::vector<long long> dp(n + 1, INF);
    dp[0] = -INF;
    size_t ans = 0;
    for (size_t i = 0; i < n; ++i) {
        auto it = std::lower_bound(dp.begin(), dp.end(), a[i]);
        if (*(it - 1) < a[i] && a[i] < *it) {
            ans = std::max(ans, size_t(it - dp.begin()));
            *it = a[i];
        }
    }
    std::cout << ans << '\n';
}