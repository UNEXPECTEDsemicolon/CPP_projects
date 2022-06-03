#include <stdio.h>

long long min(long long x, long long y) {
    if (x < y)
        return x;
    else
        return y;
}

int main() {
    size_t n;
    scanf("%zu", &n);
    auto z = new long long [n];
    auto a = new long long [n];
    auto b = new long long [n];
    auto A = new long long [n];
    auto B = new long long [n];
    auto C = new long long [n];
    scanf("%lld %lld %lld", &A[0], &B[0], &C[0]);
    z[0] = A[0];
    a[0] = B[0];
    b[0] = C[0];
    for (size_t i = 1; i < n; ++i) {
        scanf("%lld %lld %lld", &A[i], &B[i], &C[i]);
        z[i] = z[i-1] + A[i];
        if (i >= 2 && (a[i-1] == b[i-2] || a[i-1] == z[i-2] + B[i-1]) || i == 1)
            z[i] = min(z[i], a[i-1]);
        a[i] = z[i-1] + B[i];
        if (i >= 2 && b[i-1] == z[i-2] + C[i-1] || i == 1)
            a[i] = min(a[i], b[i-1]);
        b[i] = z[i-1] + C[i];
    }
    printf("%lld\n", z[n-1]);
    delete [] z;
    delete [] a;
    delete [] b;
    delete [] A;
    delete [] B;
    delete [] C;
}