#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

const size_t m = 8;
const size_t K = 1ull << m;

size_t get_digit(size_t x, size_t i) {
    return (x >> ((i - 1) * m)) % K;
}

void LSD(long long *a, size_t n, bool reverse = false) {
    auto *cnt = new size_t [K + 1];
    auto *res = new long long [n];
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 0; j < K + 1; ++j) cnt[j] = 0;
        for (size_t j = 0; j < n; ++j) ++cnt[get_digit(a[j], i)];
        for (size_t j = 1; j < K; ++j) cnt[j] += cnt[j - 1];
        for (size_t j = n - 1; j + 1 >= 1; --j) res[--cnt[get_digit(a[j], i)]] = a[j];
        if (reverse) for (size_t i = 0; i < n; ++i) a[i] = res[n - i - 1];
        else for (size_t i = 0; i < n; ++i) a[i] = res[i];
    }
    delete [] cnt;
    delete [] res;
}

void Merge(long long *a, size_t na, long long *b, size_t nb) {
    size_t n = na;
    long long *res = new long long [n];
    size_t i = 0, j = 0;
    for (; i < na && j < nb && i + j < n;) {
        for (; i < na && a[i] >= b[j] && i + j < n; i++) res[i+j] = a[i];
        for (; j < nb && a[i] < b[j] && i + j < n; j++) res[i+j] = b[j];
    }
    for (; i < na && i + j < n; i++) res[i+j] = a[i];
    for (; j < nb && i + j < n; j++) res[i+j] = b[j];
    for (i = 0; i < n; i++) a[i] = res[i];
    delete [] res;
}

int main() {
    size_t n, k;
    scanf("%zu %zu", &n, &k);
    auto *a = new long long [k * 2];
    size_t i = 0;
    for (; i < k; ++i) scanf("%lld", &a[i]);
//    LSD(a__, i, true);
    std::sort(a, a+k);
//    for (size_t i = 0; i < k; ++i) printf("%lld ", a__[i]);
//    printf("\n");
    for (i = k; i < n; i += k) {
        size_t j = 0;
        for (; i + j < n && j < k; ++j) scanf("%lld", &a[k + j]);
//        LSD(a__+k, j, true);
        std::sort(a+k, a+k+j);
        Merge(a, k, a + k, j);
//        printf("i=%zu: ", i);
//        for (size_t i = 0; i < k; ++i) printf("%lld ", a__[i]);
//        printf("\n");
    }
//    for (size_t i = 0; i < k; ++i) printf("%lld\n", a__[i]);
    delete [] a;
}