#include <stdio.h>
#include <algorithm>
#include <cmath>

template <typename T>
class DO {
private:
    size_t n;
    T *body = nullptr;
public:
    DO(size_t n): n(n) {
        size_t N = n * (ceil(log2(n)) + 1);
        body = new T [N];
        for (size_t i = 0; i < N; ++i) body[i] = 0;
    }
    ~DO() {
        delete [] body;
    }
    void update(size_t v, size_t tl, size_t tr, size_t pos, T delta) {
        if (tl == 1 && tr == n && pos % 2 == 0) delta = -delta;
        if (tl == tr) {
            body[v] = delta;
            return;
        }
        size_t tm = (tl + tr) >> 1;
        if (pos <= tm) update(v * 2, tl, tm, pos, delta);
        else update(v * 2 + 1, tm + 1, tr, pos, delta);
        body[v] = body[v*2] + body[v*2+1];
    }
    T getTar(size_t v, size_t tl, size_t tr, size_t l, size_t r) {
        if (tl == l && tr == r) return body[v];
        size_t tm = (tl + tr) >> 1;
        T ans = 0;
        if (l <= tm)
            ans += getTar(v*2, tl, tm, l, std::min(r, tm));
        if (r >= tm + 1)
            ans += getTar(v*2 + 1, tm+1, tr, std::max(l, tm+1), r);
        return ans;
    }
};

int main() {
    size_t n;
    scanf("%zu", &n);
    DO<long long> myDO(n);
    for (size_t i = 1; i <= n; ++i) {
        long long x;
        scanf("%lld", &x);
        myDO.update(1, 1, n, i, x);
    }
    size_t k;
    scanf("%zu", &k);
    for (size_t i = 0; i < k; ++i) {
        long long x, y, z;
        scanf("%lld %lld %lld", &x, &y, &z);
        if (x == 1) {
            long long res = myDO.getTar(1, 1, n, y, z);
            printf("%lld\n", (y % 2 != 0 ? res : -res));
        }
        else myDO.update(1, 1, n, y, z);
    }
}

/*
 * 1 2 3 4 5
 * # # # #
 *   # # # #
 * # # #
 *     # #
 */