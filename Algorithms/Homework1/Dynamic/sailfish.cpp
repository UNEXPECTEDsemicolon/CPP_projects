#include <stdio.h>

long long min(long long x, long long y) {
    if (x < y)
        return x;
    else
        return y;
}

long long min(long long x, long long y, long long z) {
    return min(min(x, y), z);
}

long long min(long long x, long long y, long long z, long long w) {
    return min(min(x, y, z), w);
}

int main() {
    size_t n;
    scanf("%zu", &n);
    long long A, B, C;
    auto *z = new long long [n];
    auto *a = new long long [n];
    auto *b = new long long [n];
    auto *gr = new int [n];
    scanf("%lld %lld %lld", &z[0], &a[0], &b[0]);
    for (size_t i = 1; i < n; ++i) {
        scanf("%lld %lld %lld", &A, &B, &C);
        z[i] = min(a[i-1], z[i-1] + A, gr[i]);
        a[i] = min(b[i-1], a[i-1] + A, z[i-1] + B);
        b[i] = min(b[i-1] + A, a[i-1] + B, z[i-1] + C);
    }
    printf("%lld\n", z[n-1]);
    delete [] z;
    delete [] a;
    delete [] b;
}