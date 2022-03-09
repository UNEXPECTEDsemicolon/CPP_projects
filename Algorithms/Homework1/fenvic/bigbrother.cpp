#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <set>

const long long INF = 300000;

template <typename T, typename Cmper>
class DO {
public:
    std::multiset<std::pair<T, size_t>, Cmper> *body = nullptr;
    T *a = nullptr;

    std::multiset<std::pair<T, size_t>, Cmper> merge(std::multiset<std::pair<T, size_t>, Cmper> s1,
                                                     std::multiset<std::pair<T, size_t>, Cmper> s2) {
        auto res = std::multiset<std::pair<T, size_t>, Cmper>();
        for (auto it = s1.begin(); it != s1.end(); ++it)
            res.insert(*it);
        for (auto it = s2.begin(); it != s2.end(); ++it)
            res.insert(*it);
        return res;
    }
    void build(T *a, size_t v, size_t tl, size_t tr) {
        if (tl == tr) {
            body[v].insert(std::pair<T, size_t>(a[tl-1], tl));
        }
        else {
            size_t tm = (tl + tr) >> 1;
            build(a, v * 2, tl, tm);
            build(a, v * 2 + 1, tm + 1, tr);
            body[v] = merge(body[v*2], body[v*2+1]);
        }
    }
public:
    DO(T *a, size_t n) {
        size_t N = n * (ceil(log2(n)) + 1);
        body = new std::multiset<std::pair<T, size_t>, Cmper> [N];
        this->a = new T [n];
        for (size_t i = 0; i < n; ++i) this->a[i] = a[i];
        build(a, 1, 1, n);
    }
    ~DO() {
        delete [] body;
        delete [] a;
    }
    void update(size_t v, size_t tl, size_t tr, size_t pos, T val) {
        if (!body[v].empty()) {
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
