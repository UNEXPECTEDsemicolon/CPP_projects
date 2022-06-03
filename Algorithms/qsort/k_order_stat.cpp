#include <stdio.h>
#include <stdlib.h>
#include <ctime>

size_t *partition(long long *a, size_t n, size_t x) {
    auto *res = new long long [n];
    auto ns = new size_t [3];
    ns[0] = ns[1] = ns[2] = 0;
    for (size_t i = 0; i < n; ++i) {
        if (a[i] < x) {
            res[ns[0]++] = a[i];
        }
    }
    for (size_t i = 0; i < n; ++i) {
        if (a[i] == x) {
            res[ns[0] + ns[1]++] = a[i];
        }
    }
    for (size_t i = 0; i < n; ++i) {
        if (a[i] > x) {
            res[ns[0] + ns[1] + ns[2]++] = a[i];
        }
    }
    for (size_t i = 0; i < n; ++i)
        a[i] = res[i];
    delete [] res;
    return ns;
}

long long QuickSelect(long long *a, size_t n, size_t k) {
    if (n == 1) return a[0];
    long long x = a[std::rand() % n];
    auto ns = partition(a, n, x);
    size_t n1 = ns[0], n2 = ns[1], n3 = ns[2];
    delete [] ns;
    if (k <= n1) return QuickSelect(a, n1, k);
    if (k <= n1 + n2) return x;
    return QuickSelect(a + n1 + n2, n3, k - n1 - n2);
}
int main() {
    size_t n, k;
    scanf("%zu %zu", &n, &k);
    auto *a = new long long [n];
    for (size_t i = 0; i < n; ++i)
        scanf("%lld", &a[i]);
    std::srand(std::time(nullptr));
    printf("%lld\n", QuickSelect(a, n, k + 1));
    delete [] a;
}