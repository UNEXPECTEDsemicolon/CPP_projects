#include <stdio.h>
#include <stdlib.h>

int digit(unsigned long long n, size_t k, size_t N, size_t M) {
    return (n >> (N * k) & (M - 1));
}
void radixsort(long long* l, long long* r, size_t N = 8) {
    size_t k = (32ull + N - 1ull) / N;
    size_t M = 1ull << N;
    size_t sz = r - l;
    auto b = new long long [sz];
    auto c = new long long [M];
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < M; ++j)
            c[j] = 0;
        for (long long *j = l; j < r; ++j)
            ++c[digit(*j, i, N, M)];
        for (size_t j = 1; j < M; ++j)
            c[j] += c[j - 1];
        for (long long *j = r - 1; j >= l; --j)
            b[--c[digit(*j, i, N, M)]] = *j;
        size_t cur = 0;
        for (long long *j = l; j < r; ++j)
            *j = b[cur++];
    }
//    for (long long *j = l; j < r; ++j);
    delete [] b;
    delete [] c;
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
    radixsort(a, a+i-1);
//    for (size_t i = 0; i < k; ++i) printf("%lld ", a[i]);
//    printf("\n");
    for (i = k; i < n; i += k) {
        size_t j = 0;
        for (; i + j < n && j < k; ++j) scanf("%lld", &a[k + j]);
//        LSD(a+k, j, true);
        radixsort(a+k, a+k+j-1);
//        Merge(a, k, a + k, j);
//        printf("i=%zu: ", i);
//        for (size_t i = 0; i < k; ++i) printf("%lld ", a[i]);
//        printf("\n");
    }
//    for (size_t i = 0; i < k; ++i) printf("%lld\n", a[i]);
    delete [] a;
}