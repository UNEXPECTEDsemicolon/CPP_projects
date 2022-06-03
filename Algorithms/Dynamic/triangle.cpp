#include <stdio.h>
#include <algorithm>

int main() {
    size_t n;
    scanf("%zu", &n);
    auto **t = new long long* [n];
    for (size_t i = 0; i < n; ++i) {
        t[i] = new long long [i + 1];
        for (size_t j = 0; j <= i; ++j)
            scanf("%lld", &t[i][j]);
    }
    for (size_t i = n - 2; i + 1 >= 1; --i)
        for (size_t j = 0; j <= i; ++j)
            t[i][j] += std::max(t[i+1][j], t[i+1][j+1]);
    printf("%lld\n", t[0][0]);
    for (size_t i = 0; i < n; ++i)
        delete [] t[i];
    delete [] t;
}