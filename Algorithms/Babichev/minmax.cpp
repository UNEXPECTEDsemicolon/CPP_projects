#include <stdio.h>

long abs(long x) {
    return (x >= 0 ? x : -x);
}

long max(long x, long y) {
    return (x >= y ? x : y);
}

int main () {
    size_t n, m, L;
    scanf("%zu %zu %zu", &n, &m, &L);
    long **a = new long* [n];
    long **b = new long* [m];
    for (size_t k = 0; k < n; k++) {
        a[k] = new long [L];
        for (size_t i = 0; i < L; i++)
            scanf("%ld", &a[k][i]);
    }
    for (size_t k = 0; k < m; k++) {
        b[k] = new long [L];
        for (size_t i = 0; i < L; i++)
            scanf("%ld", &b[k][i]);
    }
    size_t q;
    scanf("%zu", &q);
    for (size_t qq = 0; qq < q; qq++) {
        size_t i, j;
        scanf("%zu %zu", &i, &j);
        i--; j--;
        size_t l = 0, r = L - 1, k;
        while (r - l > 1) {
            k = (l + r) / 2;
            size_t k1 = k, k2 = k;
            for (; l <= k1 - 1 && max(a[i][k1 - 1], b[j][k1 - 1]) == max(a[i][k1], b[j][k1]); k1--);
            for (; k2 + 1 <= r && max(a[i][k2], b[j][k2]) == max(a[i][k2 + 1], b[j][k2 + 1]); k2++);
            if (k1 > l && k2 < r) {
                if (max(a[i][k2+1], b[j][k2+1]) < max(a[i][k1-1], b[j][k1-1]))
                    l = k2;
                else if (max(a[i][k2+1], b[j][k2+1]) > max(a[i][k1-1], b[j][k1-1]))
                    r = k1;
                else
                    l = r = k;
            }
            else if (k1 == l && k2 == r)
                break;
            else if (k1 == l)
                l = k2;
            else if (k2 == r)
                r = k1;
        }
        if (max(a[i][l], b[j][l]) <= max(a[i][r], b[j][r]))
            printf("%zu\n", l + 1);
        else
            printf("%zu\n", r + 1);
    }
    for (size_t nn = 0; nn < n; nn++)
        delete [] a[nn];
    delete [] a;
    for (size_t mm = 0; mm < m; mm++)
        delete [] b[mm];
    delete [] b;
}
