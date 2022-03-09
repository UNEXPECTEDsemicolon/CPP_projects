#include <stdio.h>

template <typename T>
class FenvicTree {
private:
    static size_t f(size_t x) {
        return x & (x + 1);
    }
    static size_t g(size_t x) {
        return x | (x + 1);
    }
    T _getSum(size_t x2, size_t y2, size_t z2) {
        T ans = 0;
        for (size_t i = x2; i + 1 >= 1; i = f(i) - 1)
            for (size_t j = y2; j + 1 >= 1; j = f(j) - 1)
                for (size_t k = z2; k + 1 >= 1; k = f(k) - 1)
                    ans += s[i][j][k];
        return ans;
    }
    size_t n;
    T ***s;
public:
    FenvicTree(size_t n): n(n) {
        s = new T** [n];
        for (size_t i = 0; i < n; ++i) {
            s[i] = new T* [n];
            for (size_t j = 0; j < n; ++j) {
                s[i][j] = new T[n];
                for (size_t k = 0; k < n; ++k)
                    s[i][j][k] = 0;
            }
        }
    }
    ~FenvicTree() {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j)
                delete[] s[i][j];
            delete[] s[i];
        }
        delete [] s;
    }
    T getSum(size_t x1, size_t y1, size_t z1, size_t x2, size_t y2, size_t z2) {
        T ans = _getSum(x2, y2, z2);
        if (x1) ans -= _getSum(x1 - 1, y2, z2);
        if (y1) ans -= _getSum(x2, y1 - 1, z2);
        if (z1) ans -= _getSum(x2, y2, z1 - 1);
        if (x1 && y1) ans += _getSum(x1 - 1, y1 - 1, z2);
        if (x1 && z1) ans += _getSum(x1 - 1, y2, z1 - 1);
        if (y1 && z1) ans += _getSum(x2, y1 - 1, z1 - 1);
        if (x1 && y1 && z1) ans -= _getSum(x1 - 1, y1 - 1, z1 - 1);
        return ans;
    }
    void update(size_t x, size_t y, size_t z, T delta) {
        for (size_t i = x; i < n; i = g(i))
            for (size_t j = y; j < n; j = g(j))
                for (size_t k = z; k < n; k = g(k))
                    s[i][j][k] += delta;
    }
};

int main() {
    size_t n;
    scanf("%zu", &n);
    auto ft = FenvicTree<long long>(n);
    while (true) {
        size_t m;
        scanf("%zu", &m);
        if (m == 3) break;
        if (m == 1) {
            size_t x, y, z;
            long long delta;
            scanf("%zu %zu %zu %lld", &x, &y, &z, &delta);
            ft.update(x, y, z, delta);
        } else {
            size_t x1, y1, z1, x2, y2, z2;
            scanf("%zu %zu %zu %zu %zu %zu", &x1, &y1, &z1, &x2, &y2, &z2);
            printf("%lld\n", ft.getSum(x1, y1, z1, x2, y2, z2));
        }
    }
}
