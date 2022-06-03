#include <stdio.h>

int main() {
    size_t n, q;
    scanf("%zu %zu", &n, &q);
    long long *a = new long long [n];
    for (size_t i=0; i<n; i++)
        scanf("%lld", &a[i]);
    size_t l, r;
    long long x, **labels = new long long *[2];
    labels[0] = new long long [n+1];
    labels[1] = new long long [n+1];
    for (size_t i=0; i<n; i++) {
        labels[0][i] = 0;
        labels[1][i] = 0;
    }
    for (size_t i=0; i<q; i++) {
        scanf("%zu %zu %lld", &l, &r, &x);
        labels[0][l] += x;
        labels[1][r+1] += x;
    }
    long long sum=0;
    for (size_t i=0; i<n; i++) {
        sum += labels[0][i] - labels[1][i];
        printf("%lld ", a[i]+sum);
    }
    delete [] a;
    delete [] labels[0];
    delete [] labels[1];
    delete [] labels;
}
