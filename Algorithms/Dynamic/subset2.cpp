#include <iostream>
#include <vector>

const long long INF = (1ull<<31) - 1;

struct MyPair {
    int first;
    long long second;
    MyPair(int first, long long second): first(first), second(second) {}

    bool operator<(const MyPair& right) const {
        if (second < right.second)
            return true;
        if (second > right.second)
            return false;
        return first < right.first;
    }
};

int main() {
    int n;
    std::cin >> n;
    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i)
        std::cin >> a[i];
    auto dp = std::vector<MyPair>(n+1, {0, INF});
    auto way = std::vector<int>(n, -1);
    int best_k = 0;
    int best_end = -1;
    for (int i = 0; i < n; ++i) {
        auto k_it = std::lower_bound(dp.begin(), dp.end(), MyPair(i, -a[i]));
        if (k_it->second == INF) {
            ++best_k;
            best_end = i;
        }
        if ((k_it) != dp.begin())
            way[i] = (k_it - 1)->first;
        *k_it = {i, -a[i]};
    }
    auto ans = std::vector<int>(best_k);
    for (int i = best_end, k = best_k - 1; i >= 0 && k >= 0; i = way[i], --k)
        ans[k] = i + 1;
    std::cout << ans.size() << '\n';
    for (auto it = ans.begin(); it != ans.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';
}