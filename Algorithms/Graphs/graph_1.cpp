#include <iostream>

int main() {
    size_t N;
    std::cin >> N;
    size_t ans = 0;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            bool x;
            std::cin >> x;
            if (j > i && x)
                ++ans;
        }
    }
    std::cout << ans << '\n';
}