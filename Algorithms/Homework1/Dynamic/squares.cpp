#include <stdio.h>
#include <algorithm>
#include <stack>

int main() {
    long long  n;
    scanf("%lld", &n);
    auto **a = new int* [n];
    for (long long i = 0; i < n; ++i) {
        a[i] = new int [n];
        for (long long j = 0; j < n; ++j) {
            scanf("%d", &a[i][j]);
            a[i][j] = !a[i][j];
        }
    }
    auto **dp = new long long* [n];
    for (long long i = 0; i < n; ++i) {
        dp[i] = new long long[n];
        for (long long j = 0; j < n; ++j) {
            if (a[i][j] == 1)
                dp[i][j] = i;
            else if (i == 0)
                dp[i][j] = -1;
            else
                dp[i][j] = dp[i - 1][j];
        }
    }
    auto **lefts = new long long* [n];
    for (long long i = 0; i < n; ++i) {
        lefts[i] = new long long [n];
        auto s = std::stack<long long>();
        for (long long j = 0; j < n; ++j) {
            while (!s.empty() && dp[i][s.top()] <= dp[i][j])
                s.pop();
            lefts[i][j] = (!s.empty() ? s.top() : -1);
            s.push(j);
        }
    }
    auto **rights = new long long* [n];
    for (long long i = 0; i < n; ++i) {
        rights[i] = new long long [n];
        auto s = std::stack<long long>();
        for (long long j = n - 1; j >= 0; --j) {
            while (!s.empty() && dp[i][s.top()] <= dp[i][j])
                s.pop();
            rights[i][j] = (!s.empty() ? s.top() : n);
            s.push(j);
        }
    }
    long long ans = 0;
    for (long long  i = 0; i < n; ++i) {
        for (long long j = 0; j < n; ++j) {
            long long top = dp[i][j];
            ans = std::max(ans, std::min(i - top, rights[i][j] - lefts[i][j] - 1));
        }
    }
    printf("%lld\n", ans);
    for (long long i = 0; i < n; ++i) {
        delete [] a[i];
        delete [] dp[i];
        delete [] lefts[i];
        delete [] rights[i];
    }
    delete [] a;
    delete [] dp;
    delete [] lefts;
    delete [] rights;
}
