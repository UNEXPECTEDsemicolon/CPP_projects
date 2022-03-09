#include <stdio.h>
#include <stdlib.h>

int cmp_int(const void *x, const void *y) {
    long long xl = (*((long long **)x))[0];
    long long yl = (*((long long **)y))[0];
    if (xl < yl) return -1;
    if (xl == yl) return 0;
    return 1;
}

int main() {
    size_t n;
    scanf("%zu", &n);
    long long **lr = new long long* [2 * n];
    for (size_t i = 0; i < 2 * n; i++)
        lr[i] = new long long [2];
    for (size_t i = 0; i < 2 * n; i += 2) {
        scanf("%lld %lld", &lr[i][0], &lr[i + 1][0]);
        lr[i][1] = 1;
        lr[i+1][1] = -1;
    }
    qsort(lr, 2 * n, sizeof(long long*), cmp_int);
    long long ans = 0, sum = 0, last_ind = 0;
    for (size_t i = 0; i < 2 * n; i++) {
        if (sum == 1)
            ans += lr[i][0] - last_ind;
        sum += lr[i][1];
        last_ind = lr[i][0];
    }
    printf("%lld\n", ans);
    for (size_t i = 0; i < 2 * n; i++)
        delete [] lr[i];
    delete [] lr;
}
