#include <stdio.h>

size_t Merge(long long *a, size_t na, long long *b, size_t nb) {
    long long *res = new long long [na + nb];
    size_t i = 0, j = 0;
    size_t inv = 0;
    for (; i < na && j < nb;) {
        for (; i < na && a[i] <= b[j]; i++)
            res[i+j] = a[i];
        for (; j < nb && a[i] > b[j]; j++) {
            res[i+j] = b[j];
            inv += (na - i);
        }
    }
    for (; i < na; i++)
        res[i+j] = a[i];
    for (; j < nb; j++)
        res[i+j] = b[j];
    for (i = 0; i < na + nb; i++)
        a[i] = res[i];
    delete [] res;
    return inv;
}

size_t GetInv(long long *a, size_t n) {
    if (n == 1)
        return 0;
    size_t result = GetInv(a, n/2) + GetInv(a + n/2, n - n/2);

    return result + Merge(a, n/2, a + n/2, n - n/2);
}

int main() {
    size_t n;
    scanf("%zu", &n);
    long long *a = new long long [n];
    for (size_t i=0; i<n; i++)
        scanf("%lld", &a[i]);
    printf("%zu\n", GetInv(a, n));
    delete [] a;
}
