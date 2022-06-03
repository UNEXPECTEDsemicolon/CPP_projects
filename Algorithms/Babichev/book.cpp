#include <stdio.h>

long long max(const long long *v, long long n) {
    long long ma = -1;
    for (long long i=0; i<n; i++)
        if (v[i] > ma)
            ma = v[i];
        return ma;
}

long long get_k(long long m, long long n, const long long *v) {
    long long my_k = 0, cur_sum = 0;
    for (long long i = 0; i < n; i++) {
        if (cur_sum + v[i] <= m)
            cur_sum += v[i];
        else {
            my_k++;
            cur_sum = v[i];
        }
    }
    return ++my_k;
}

int main() {
    long long n, k;
    scanf("%lld %lld", &k, &n);
    long long *v = new long long [n];
    for (long long i = 0; i < n; i++)
        scanf("%lld", &v[i]);
    long long l = max(v, n), r = 10000 * n, m, my_k;
    while (r-l>1) {
        m = (l + r) / 2;
        my_k = get_k(m, n, v);
        if (my_k > k)
            l = m;
        else
            r = m;
    }
    if (get_k(l, n, v) <= k)
        m = l;
    else
        m = r;
    printf("%lld", m);
    delete [] v;
}
