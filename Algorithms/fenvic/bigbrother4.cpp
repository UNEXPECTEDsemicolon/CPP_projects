#include <stdio.h>
#include <algorithm>
#include <cmath>

template <typename T, typename Tar, typename invTar>
class DO {
public:
    T *body_min = nullptr;
    T *body_max = nullptr;
public:
    DO(size_t n) {
        size_t N = n * (ceil(log2(n)) + 1);
        body_min = new T [N];
        body_max = new T [N];
        for (size_t i = 0; i < N; ++i) {
            body_min[i] = invTar()(-100000ll, 100000ll);
            body_max[i] = Tar()(-100000ll, 100000ll);
        }
    }
    ~DO() {
        delete [] body_min;
        delete [] body_max;
    }
    void update(size_t v, size_t tl, size_t tr, size_t pos, T delta) {
        if (tl == tr) {
            body_min[v] = delta;
            body_max[v] = delta;
            return;
        }
        size_t tm = (tl + tr) >> 1;
        if (pos <= tm) update(v * 2, tl, tm, pos, delta);
        else update(v * 2 + 1, tm + 1, tr, pos, delta);
        body_min[v] = Tar()(body_min[v*2], body_min[v*2+1]);
        body_max[v] = invTar()(body_max[v*2], body_max[v*2+1]);
    }
    size_t getTar(size_t v, size_t tl, size_t tr, size_t l, size_t r, T x) {
        if (body_max[v] < x) return -1;
        if (tl == l && tr == r) {
            if (body_min[v] >= x) return tl;
            else if (r - l == 0) return -1;
        }
        size_t tm = (tl + tr) >> 1;
        size_t ans = -1;
        if (l <= tm)
            ans = std::min(ans, getTar(v*2, tl, tm, l, std::min(r, tm), x));
        if (ans == -1 && r >= tm + 1)
            ans = std::min(ans, getTar(v*2 + 1, tm+1, tr, std::max(l, tm+1), r, x));
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
    size_t n, m;
    scanf("%zu %zu", &n, &m);
    DO<long long, MinTar, MaxTar> maxDO(n);
    for (size_t i = 1; i <= n; ++i) {
        long long a;
        scanf("%lld", &a);
        maxDO.update(1, 1, n, i, a);
    }
    for (size_t i = 0; i < m; ++i) {
        int act;
        scanf("%d", &act);
        size_t I;
        long long x;
        scanf("%zu %lld", &I, &x);
        if (act > 0) {
            auto ans = maxDO.getTar(1, 1, n, I, n, x);
            printf("%lld\n", (ans != -1 ? (long long)ans : -1));
        }
        else {
            maxDO.update(1, 1, n, I, x);
        }
    }
}