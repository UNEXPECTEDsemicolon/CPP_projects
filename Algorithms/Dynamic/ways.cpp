#include <stdio.h>

int main() {
    size_t n, m;
    scanf("%zu %zu", &n, &m);
    ++n; ++m;
    auto **t = new size_t* [n];
    for (size_t i = 0; i < n; ++i) {
        t[i] = new size_t [m];
        for (size_t j = 0; j < m; ++j) t[i][j] = 0;
    }
    t[0][0] = 1;
    for (size_t i = 1; i < n; ++i)
        t[i][0] += t[i-1][0];
    for (size_t i = 1; i < m; ++i)
        t[0][i] += t[0][i-1];
    for (size_t i = 1; i < n; ++i)
        for (size_t j = 1; j < m; ++j)
            t[i][j] += t[i-1][j] + t[i][j-1];
    printf("%zu\n", t[n-1][m-1]);
    for (size_t i = 0; i < n; ++i)
        delete [] t[i];
    delete [] t;
}