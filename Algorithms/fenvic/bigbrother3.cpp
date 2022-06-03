#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <set>

const long long INF = 300000;

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
