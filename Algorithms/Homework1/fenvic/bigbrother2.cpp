#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <set>

const long long INF = 300000;

template <typename T, typename Cmper>
class DO {
public:
    size_t N;
    std::pair<T, size_t> **body = nullptr;
    T *a = nullptr;

    std::pair<T, size_t> *merge(std::pair<T, size_t> *s1, size_t s1n,
                                std::pair<T, size_t> *s2, size_t s2n) {
        auto res = new std::pair<T, size_t> [s1n + s2n];
        size_t i = 0, j = 0;
        while (i < na && j < nb) {
            for (; i < na && a[i] <= b[j]; i++) res[i+j] = a[i];
            for (; j < nb && a[i] > b[j]; j++) res[i+j] = b[j];
        }
        for (; i < na; i++) res[i+j] = a[i];
        for (; j < nb; j++) res[i+j] = b[j];
        return res;
    }
    void build(T *a, size_t v, size_t tl, size_t tr) {
        if (tl == tr) {
            body[v] = new std::pair<T, size_t> [1];
            body[v][0] = std::pair<T, size_t>(a[tl-1], tl);
        }
        else {
            size_t tm = (tl + tr) >> 1;
            build(a, v * 2, tl, tm);
            build(a, v * 2 + 1, tm + 1, tr);
            body[v] = merge(body[v*2], tm - tl + 1, body[v*2+1], tr - tm);
        }
    }
    void replace(size_t v, T x, T y) {
        size_t l = 0; r = n;
    }
public:
    DO(T *a, size_t n) {
        N = n * (ceil(log2(n)) + 1);
        body = new std::pair<T, size_t>* [N];
        for (size_t i =0; i < N; ++i) body[i] = nullptr;
        this->a = new T [n];
        for (size_t i = 0; i < n; ++i) this->a[i] = a[i];
        build(a, 1, 1, n);
    }
    ~DO() {
        for (size_t i =0; i < N; ++i) if (body[i] != nullptr) delete body[i];
        delete [] body;
        delete [] a;
    }
    void update(size_t v, size_t tl, size_t tr, size_t pos, T val) {
        if (body[v] != nullptr) {
            replace(v, a[pos - 1], val);
            body[v].erase(body[v].find(std::pair<T, size_t>(a[pos - 1], pos)));
            body[v].insert(std::pair<T, size_t>(val, pos));
        }
        if (tl == tr) {
            a[pos-1] = val;
            return;
        }
        size_t tm = (tl + tr) >> 1;
        if (pos <= tm) update(v * 2, tl, tm, pos, val);
        else update(v * 2 + 1, tm + 1, tr, pos, val);
    }
    std::pair<T, size_t> getTar(size_t v, size_t tl, size_t tr, size_t l, size_t r, T x) {
        if (l > r) return std::pair<T, size_t>(INF, 0);
        if (tl == l && tr == r) {
            auto pos = body[v].lower_bound(std::pair<T, size_t>(x, 0));
            if (pos != body[v].end()) return *pos;
            return std::pair<T, size_t>(INF, 0);
        }
        size_t tm = (tl + tr) >> 1;
        auto ans1 = getTar(v*2, tl, tm, l, std::min(r, tm), x);
        auto ans2 = getTar(v*2 + 1, tm+1, tr, std::max(l, tm+1), r, x);
        if (Cmper()(ans1, ans2)) return ans1;
        else return ans2;
    }
};

class MyCmper {
public:
    bool operator() (const std::pair<long long, size_t> &x, const std::pair<long long, size_t> &y) const {
        if (x.first != y.first) return x.first < y.first;
        else return x.second < y.second;
    }
};

int main() {
    size_t n, m;
    scanf("%zu %zu", &n, &m);
    auto a = new long long [n];
    for (size_t i = 0; i < n; ++i) scanf("%lld", &a[i]);
    auto myDO = DO<long long, MyCmper>(a, n);
    for (size_t q = 0; q < m; ++q) {
        int act;
        size_t i;
        long long x;
        scanf("%d %zu %lld", &act, &i, &x);
        if (act == 0) {
            myDO.update(1, 1, n, i, x);
        } else {
            auto res = myDO.getTar(1, 1, n, i, n, x);
            printf("%lld\n", (res.first != INF ? (long long)(res.second) : -1));
        }
    }
    delete [] a;
}
