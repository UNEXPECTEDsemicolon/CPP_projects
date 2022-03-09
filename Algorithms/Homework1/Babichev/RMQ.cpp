#include <stdio.h>
#include <cmath>
#include <algorithm>

int main() {
    size_t n, m;
    scanf("%zu %zu", &n, &m);
    size_t K = ceil(log2(n));
    int *a = new int [n];
    for (size_t i = 0; i < n; ++i) scanf("%d", &a[i]);
    int *logs = new int [n + 1];
    for (size_t i = 1; i < n + 1; ++i) logs[i] = floor(log2(i));
    int **sparse = new int * [K];
    sparse[0] = new int [n];
    for (size_t i = 0; i < n; ++i) sparse[0][i] = a[i];
    size_t cur_k = n;
    for (size_t i = 1; i < K; ++i) {
        cur_k -= (1 << (i - 1));
        sparse[i] = new int [cur_k];
        for (size_t j = 0; j < cur_k; j++) {
            sparse[i][j] = std::max(sparse[i-1][j], sparse[i - 1][j + (1 << (i-1))]);
//            printf("%d ", sparse[i][j]);
        }
//        printf("\n");
    }
    size_t l, r, k;
    for (size_t i = 0; i < m; ++i) {
        scanf("%zu %zu", &l, &r);
//        l--; r--;
        k = logs[r - l + 1];
        printf("%d\n", std::max(sparse[k][l], sparse[k][r - (1 << k) + 1]));
    }
    for (size_t i = 0; i < K; ++i) delete [] sparse[i];
    delete [] sparse;
    delete [] logs;
    delete [] a;
}