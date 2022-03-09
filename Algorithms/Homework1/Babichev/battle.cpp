#include <stdio.h>

long long get_comp(long long m, long long p, long long q, long long *a, size_t n) {
    long long comp_m = 0;
    for (size_t i = 0; i < n; i++)
        if (a[i] - m * q > 0) {
            comp_m += (a[i] - m * q + (p - q - 1)) / (p - q);
        }
    return comp_m;
}

int main() {
    size_t n;
    long long q, p;
    scanf("%zu %lld %lld", &n, &p, &q);
    long long *a = new long long [n];
    for (size_t i = 0; i < n; i++)
        scanf("%lld", &a[i]);
    if (p == q) {
        long long maxa = -1;
        for (size_t i = 0; i < n; i++)
            if (a[i] > maxa)
                maxa = a[i];
        printf("%lld\n", (maxa + p - 1) / p);
        delete [] a;
        return 0;
    }
    long long l = 0, r = 1000000000ll, m;
    while (r - l > 1) {
        m = (l + r) / 2;
        long long comp_m = get_comp(m, p, q, a, n);
        if (comp_m > m)
            l = m;
        else if (comp_m < m)
            r = m;
        else {
            printf("%lld\n", m);
            delete [] a;
            return 0;
        }
    }
    if (get_comp(l, p, q, a, n) == l)
        printf("%lld\n", l);
    else
        printf("%lld\n", r);
    delete [] a;
}
