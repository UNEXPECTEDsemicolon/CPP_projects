#include <stdio.h>
#include <algorithm>
#include <cmath>

template <typename T, typename Tar, typename invTar>
class DO {
public:
    T *body = nullptr;
public:
    DO(size_t n) {
        size_t N = n * (ceil(log2(n)) + 1);
        body = new T [N];
        for (size_t i = 0; i < N; ++i)
            body[i] = invTar()(-100000ll, 100000ll);
    }
    ~DO() {
        delete [] body;
    }
    void update(size_t v, size_t tl, size_t tr, size_t pos, T delta) {
        if (tl == tr) {
            body[v] = delta;
            return;
        }
        size_t tm = (tl + tr) >> 1;
        if (pos <= tm) update(v * 2, tl, tm, pos, delta);
        else update(v * 2 + 1, tm + 1, tr, pos, delta);
        body[v] = Tar()(body[v*2], body[v*2+1]);
    }
    T getTar(size_t v, size_t tl, size_t tr, size_t l, size_t r) {
        if (tl == l && tr == r) return body[v];
        size_t tm = (tl + tr) >> 1;
        T ans = invTar()(-100001ll, 100001ll);
        if (l <= tm)
            ans = Tar()(ans, getTar(v*2, tl, tm, l, std::min(r, tm)));
        if (r >= tm + 1)
            ans = Tar()(ans, getTar(v*2 + 1, tm+1, tr, std::max(l, tm+1), r));
        return ans;
    }
};

struct MinTar {
    long long operator() (long long x, long long y) {
        return std::min(x, y);
    }
};

struct MaxTar {
    long long operator() (long long x, long long y) {
        return std::max(x, y);
    }
};

int main() {
    size_t n = 100010;
    DO<long long, MinTar, MaxTar> minDO(n);
    DO<long long, MaxTar, MinTar> maxDO(n);
    for (size_t i = 1; i <= n; ++i) {
        long long j = i;
        long long delta = (j*j % 12345) + (j*j % 23456) * j % 23456;
        minDO.update(1, 1, n, i, delta);
        maxDO.update(1, 1, n, i, delta);
    }
    size_t k;
    scanf("%zu", &k);
    for (size_t i = 0; i < k; ++i) {
        long long x, y;
        scanf("%lld %lld", &x, &y);
        if (x > 0) printf("%lld\n", maxDO.getTar(1, 1, n, x, y) -
                    minDO.getTar(1, 1, n, x, y));
        else if (x < 0) {
            maxDO.update(1, 1, n, -x, y);
            minDO.update(1, 1, n, -x, y);
        }
    }
}