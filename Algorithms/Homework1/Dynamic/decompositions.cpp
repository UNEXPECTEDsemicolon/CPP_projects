#include <stdio.h>

long long calc(size_t n, size_t k, long long **t) {
    if (k == 0) {
        if (n == 0) return 1;
        else return 0;
    }
    if (t[n][k] != -1) return t[n][k];
    if (k <= n) t[n][k] = calc(n, k-1, t) + calc(n-k, k, t);
    else t[n][k] = calc(n, n, t);
    return t[n][k];
}

int main() {
    size_t n;
    scanf("%zu", &n);
    auto **t = new long long* [n + 1];
    for (size_t i = 0; i <= n; ++i) {
        t[i] = new long long [n + 1];
        for (size_t j = 0; j <= n; ++j) t[i][j] = -1;
    }
    printf("%lld\n", calc(n, n, t));
    for (size_t i = 0; i <= n; ++i) delete [] t[i];
    delete [] t;
}