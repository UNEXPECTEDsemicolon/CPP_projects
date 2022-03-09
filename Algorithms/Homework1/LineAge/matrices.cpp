#include <stdio.h>

int main() {
    size_t m1, n1, m2, n2;
    scanf("%zu %zu %zu %zu", &m1, &n1, &m2, &n2);
    auto **a = new long long* [m1];
    for (size_t i = 0; i < m1; ++i) {
        a[i] = new long long [n1];
        for (size_t j = 0; j < n1; ++j)
            scanf("%lld", &a[i][j]);
    }
    auto **b = new long long* [m2];
    for (size_t i = 0; i < m2; ++i) {
        b[i] = new long long [n2];
        for (size_t j = 0; j < n2; ++j)
            scanf("%lld", &b[i][j]);
    }
    if (n1 != m2)
        printf("-1\n");
    else {
        for (size_t i = 0; i < m1; ++i) {
            for (size_t j = 0; j < n2; ++j) {
                long long res = 0;
                for (size_t k = 0; k < n1; ++k)
                    res += a[i][k] * b[k][j];
                printf("%lld ", res);
            }
            printf("\n");
        }
    }
    for (size_t i = 0; i < m1; ++i)
        delete [] a[i];
    for (size_t i = 0; i < m2; ++i)
        delete [] b[i];
    delete [] a;
    delete [] b;
}