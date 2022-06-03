#include <stdio.h>
#include <algorithm>

template <typename T>
class FenvicTree {
private:
    static size_t f(size_t x) {
        return x & (x + 1);
    }
    static size_t g(size_t x) {
        return x | (x + 1);
    }
    T _getSum(size_t x2) {
        T ans = 0;
        for (size_t i = x2; i + 1 >= 1; i = f(i) - 1)
            ans += s[i];
        return ans;
    }
    size_t n;
    T *s;
public:
    FenvicTree(size_t n): n(n) {
        s = new T [n];
        for (size_t i = 0; i < n; ++i)
            s[i] = 0;
    }
    ~FenvicTree() {
        delete [] s;
    }
    T getSum(size_t x1, size_t x2) {
        T ans = _getSum(x2);
        if (x1) ans -= _getSum(x1 - 1);
        return ans;
    }
    void update(size_t x, T delta) {
        for (size_t i = x; i < n; i = g(i))
            s[i] += delta;
    }
};

int cmp_left(const void* a1, const void* a2) {
    return (*(const long long**)a2)[0] - (*(const long long**)a1)[0];
}

int cmp_int(const void * p1, const void * p2) {
    return *(const int*)p1 - *(const int*)p2;
}

size_t get_ind(long long x, size_t n, long long *right) {
    size_t l = 0, r = n - 1;
    while (l + 1 < r) {
        size_t m = (l + r) / 2;
        if (right[m] < x)
            r = m;
        else if (right[m] > x)
            l = m;
        else
            return m;
    }
    if (right[r] == x)
        return r;
    else
        return l;
}

int main() {
    size_t n;
    scanf("%zu", &n);
    auto **a = new long long* [n];
    for (size_t i = 0; i < n; ++i) {
        a[i] = new long long [2];
        scanf("%lld %lld", &a[i][0], &a[i][1]);
    }
    qsort(a, n, sizeof(long long*), cmp_left);
    auto *right = new long long [n];
    for (size_t i = 0; i < n; ++i)
        right[i] = a[i][1];
    qsort(right, n, sizeof(long long), cmp_int);
    auto ft = FenvicTree<long long>(n);
    for (size_t i = 0; i < n; ++i)
        ft.update(i, 1);
    long long ans = 0;
    for (size_t i = 0; i < n; ++i) {
        auto ind = get_ind(a[i][1], n, right);
        ans += ft.getSum(0, ind);
        ft.update(ind, -1);
    }
    printf("%lld\n", ans);
    for (size_t i = 0; i < n; ++i)
        delete [] a[i];
    delete [] a;
    delete [] right;
}