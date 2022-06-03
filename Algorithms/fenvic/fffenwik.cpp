#include <stdio.h>
#include <algorithm>
#include <vector>

struct Point {
    size_t x = 0, y = 0;
    long long w = 0;
    Point() {};
    Point(size_t x, size_t y, long long w): x(x), y(y), w(w) {};
};

bool pntCmprerX (const Point &a, const Point &b) {
    if (a.x != b.x) return a.x < b.x;
    else return a.y < b.y;
}

bool pntCmprerY (const Point &a, const Point &b) {
    if (a.y != b.y) return a.y < b.y;
    else return a.x < b.x;
}

template <typename T>
class FenvicTree {
private:
    size_t n;
    std::vector<Point> points;
    std::vector<size_t> x;
    std::vector<std::vector<size_t>> yy;
    std::vector<std::vector<T>> s;
    static size_t f(size_t x) {
        return x & (x + 1);
    }
    static size_t g(size_t x) {
        return x | (x + 1);
    }
    size_t findInPoints(size_t rx, size_t ry) {
        size_t l = 0, r = n - 1;
        while (r > l + 1) {
            size_t m = (l + r) >> 1;
            if (points[m].x < rx || points[m].x == rx && points[m].y < ry) l = m;
            else if (points[m].x > rx || points[m].x == rx && points[m].y > ry) r = m;
            else return m;
        }
        if (points[r].x < rx) return r;
        else if (points[l].x > rx) return -1;
        else if (points[r].x > rx) return l;
        else if (points[l].x == points[r].x && points[l].y == ry) return l;
        else return r;
    }
    size_t findTrueInd(const std::vector<size_t> &a, size_t ar) {
        size_t l = 0, r = a.size() - 1;
        while (r > l + 1) {
            size_t m = (l + r) >> 1;
            if (a[m] <= ar) l = m;
            else if (a[m] > ar) r = m;
        }
        if (a[r] <= ar) return r;
        else if (a[l] <= ar) return l;
        else return -1;
    }
public:
    FenvicTree(size_t n, const std::vector<Point> &orig_points): n(n), points(orig_points) {
        std::sort(points.begin(), points.end(), pntCmprerY);
        std::sort(points.begin(), points.end(), pntCmprerX);
        x = std::vector<size_t>(n);
        for (size_t i = 0; i < n; ++i) {
            x[i] = points[i].x;
        }
        x.erase(std::unique(x.begin(), x.end()), x.end());
        yy = std::vector<std::vector<size_t>> (n);
        s = std::vector<std::vector<T>>(n);
        for (size_t i = 0; i < n; ++i) {
            size_t k = i + 1 - f(i);
            auto p = std::vector<Point>(points.begin() + f(i), points.begin() + f(i) + k);
            std::sort(p.begin(), p.end(), pntCmprerY);
            yy[i] = std::vector<size_t>(k);
            for (size_t j = 0; j < k; ++j) yy[i][j] = p[j].y;
            auto pr_sums = std::vector<T>(k, 0);
            pr_sums[0] = p[0].w;
            for (size_t j = 1; j < k; ++j)
                pr_sums[j] = pr_sums[j-1] + p[j].w;
            s[i] = std::vector<T>(k, 0);
            for (size_t j = 0; j < k; ++j)
                s[i][j] = pr_sums[j] - (f(j) ? pr_sums[f(j)-1] : 0);

        }
    }
    ~FenvicTree() {}
    T getSum(size_t rx, size_t ry) {
        rx = findInPoints(rx, ry);
        T ans = 0;
        for (size_t i = rx; i + 1 >= 1; i = f(i) - 1) {
            auto new_ry = findTrueInd(yy[i], ry);
            if (new_ry + 1)
                for (size_t j = new_ry; j + 1 >= 1; j = f(j) - 1)
                    ans += s[i][j];
        }
        return ans;
    }
    void update(size_t rx, size_t ry, T delta) {
        rx = findInPoints(rx, ry);
        for (size_t i = rx; i < n; i = g(i)) {
            auto new_ry = findTrueInd(yy[i], ry);
            if (new_ry + 1)
                for (size_t j = new_ry; j < s[i].size(); j = g(j))
                    s[i][j] += delta;
        }
    }
};

int main() {
    size_t n;
    scanf("%zu", &n);
    auto points = std::vector<Point>(n);
    for (size_t i = 0; i < n; ++i) {
        scanf("%zu %zu %lld", &points[i].x, &points[i].y, &points[i].w);
    }
    auto ft = FenvicTree<long long>(n, points);
    size_t m;
    scanf("%zu", &m);
    for (size_t q = 0; q < m; ++q) {
        char act;
        scanf("\n%c", &act);
        if (act == 'g') {
            scanf("%*2s");
            size_t rx, ry;
            scanf("%zu %zu", &rx, &ry);
            printf("%lld\n", ft.getSum(rx, ry)); // -1
        } else if (act == 'c'){
            scanf("%*5s");
            size_t i;
            long long z;
            scanf("%zu %lld", &i, &z);
            ft.update(points[i-1].x, points[i-1].y, z - points[i-1].w);
            points[i-1].w = z;
        }
    }
}
