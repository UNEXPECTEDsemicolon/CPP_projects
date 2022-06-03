#include <stdio.h>

int main() {
    size_t m, n, d;
    scanf("%zu %zu %zu", &m, &n, &d);
    auto ***a = new long long** [d];
    auto ***s = new long long** [d];
    for (size_t i = 0; i < d; ++i) {
        a[i] = new long long* [m];
        s[i] = new long long* [m];
        for (size_t j = 0; j < m; ++j) {
            a[i][j] = new long long [n];
            s[i][j] = new long long [n];
            for (size_t k = 0; k < n; ++k) {
                scanf("%lld", &a[i][j][k]);
                s[i][j][k] = a[i][j][k] + (i && j && k ? s[i-1][j-1][k-1] : 0);
                for (size_t i2 = 0; i2 < i; ++i2)
                    for (size_t j2 = 0; j2 < j; ++j2)
                        s[i][j][k] += a[i2][j2][k];
                for (size_t i2 = 0; i2 < i; ++i2)
                    for (size_t k2 = 0; k2 < k; ++k2)
                        s[i][j][k] += a[i2][j][k2];
                for (size_t j2 = 0; j2 < j; ++j2)
                    for (size_t k2 = 0; k2 < k; ++k2)
                        s[i][j][k] += a[i][j2][k2];
                for (size_t i2 = 0; i2 < i; ++i2)
                    s[i][j][k] += a[i2][j][k];
                for (size_t j2 = 0; j2 < j; ++j2)
                    s[i][j][k] += a[i][j2][k];
                for (size_t k2 = 0; k2 < k; ++k2)
                    s[i][j][k] += a[i][j][k2];
            }
        }
    }
    long long ans = -5000000ll;
    for (size_t i2 = 0; i2 < d; ++i2) {
        for (size_t i1 = 0; i1 <= i2; ++i1) {
            for (size_t j2 = 0; j2 < m; ++j2) {
                for (size_t j1 = 0; j1 <= j2; ++j1) {
                    for (size_t k2 = 0; k2 < n; ++k2) {
                        for (size_t k1 = 0; k1 <= k2; ++k1) {
                            long long sum = s[i2][j2][k2];
                            if (i1)
                                sum -= s[i1 - 1][j2][k2];
                            if (j1)
                                sum -= s[i2][j1 - 1][k2];
                            if (k1)
                                sum -= s[i2][j2][k1 - 1];
                            if (i1 && j1)
                                sum += s[i1 - 1][j1 - 1][k2];
                            if (i1 && k1)
                                sum += s[i1 - 1][j2][k1 - 1];
                            if (j1 && k1)
                                sum += s[i2][j1 - 1][k1 - 1];
                            if (i1 && j1 && k1)
                                sum -= s[i1 - 1][j1 - 1][k1 - 1];
                            if (sum > ans)
                                ans = sum;
                        }
                    }
                }
            }
        }
    }
    printf("%lld\n", ans);
    for (size_t i = 0; i < d; ++i) {
        for (size_t j = 0; j < m; ++j) {
            delete [] a[i][j];
            delete [] s[i][j];
        }
        delete [] a[i];
        delete [] s[i];
    }
    delete [] a;
    delete [] s;
}