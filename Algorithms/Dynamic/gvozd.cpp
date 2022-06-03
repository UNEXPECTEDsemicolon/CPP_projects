#include <stdio.h>
#include <algorithm>
#include <stdlib.h>

int main() {
    const long long INF = 10000000000;
    size_t n;
    scanf("%zu", &n);
    auto *x = new long long [n + 1];
    for (size_t i = 0; i < n; ++i) {
        scanf("%lld", &x[i]);
    }
    std::sort(x, x + n);
    auto *a = new long long [n];
    auto *b = new long long [n];
    a[0] = INF;
    b[0] = 0;
    for (size_t i = 1; i < n; ++i) {
        a[i] = std::min(a[i-1], b[i-1]) + (x[i] - x[i-1]);
        b[i] = a[i-1];
    }
    printf("%lld\n", a[n-1]);
    delete [] x;
    delete [] a;
    delete [] b;
}