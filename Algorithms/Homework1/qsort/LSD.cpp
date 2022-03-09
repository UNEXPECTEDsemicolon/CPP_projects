#include <stdio.h>

const size_t m = 8;
const size_t k = 1ull << m;

size_t get_digit(size_t x, size_t i) {
    return (x >> ((i - 1) * m)) % k;
}

void LSD(size_t *a, size_t n) {
    auto *cnt = new size_t [k + 1];
    auto *res = new size_t [n];
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 0; j < k + 1; ++j) cnt[j] = 0;
        for (size_t j = 0; j < n; ++j) ++cnt[get_digit(a[j], i)];
        for (size_t j = 1; j < k; ++j) cnt[j] += cnt[j - 1];
        for (size_t j = n - 1; j + 1 >= 1; --j)
            res[--cnt[get_digit(a[j], i)]] = a[j];
        for (size_t i = 0; i < n; ++i) a[i] = res[i];
    }
    delete [] cnt;
    delete [] res;
}

int main() {
    size_t n;
    scanf("%zu", &n);
    auto *a = new size_t [n];
    for (size_t i = 0; i < n; ++i) scanf("%zu", &a[i]);
    LSD(a, n);
    for (size_t i = 0; i < n; ++i) printf("%zu ", a[i]);
    printf("\n");
    delete [] a;
}