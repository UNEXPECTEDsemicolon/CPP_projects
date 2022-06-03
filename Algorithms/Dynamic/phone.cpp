#include <iostream>
#include <cmath>

double factorroot(int k, int j) {
    return (j == 0) ? 1 : factorroot(k, j - 1) * std::pow(j, 1.0/k);
}

int solve() {
    int n, k;
    std::cin >> n >> k;
    if (n == 1)
        return 0;
    if (k == 0)
        return -1;
    if (k == 1) {
        return n - 1;
    }
    int log2n = std::ceil(std::log2(n));
    if (k >= log2n)
        return log2n;
    else {
        int temp = std::floor(factorroot(k, k) *  std::pow(n, 1.0/k));
        int left = std::max(temp - k, 1);
        int right = temp + k + 1;
        for (int m = left; m <= right; ++m) {
            int C = 1;
            int sum = C;
            for (int j = 1; j <= std::min(k, m); ++j) {
                C = C * (m - j + 1) / j;
                sum += C;
            }
            if (sum >= n) {
                return m;
            }
        }
    }
    return 0;
}

int main() {
    std::cout << solve() << '\n';
}