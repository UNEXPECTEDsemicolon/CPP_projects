#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    auto power = std::vector<int>(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int x;
            std::cin >> x;
            if (x && i != j) {
                power[i] += x;
            }
        }
    }
    for (auto p: power)
        if (p % 2 != 0) {
            std::cout << "NO\n";
            return 0;
        }
    std::cout << "YES\n";
}