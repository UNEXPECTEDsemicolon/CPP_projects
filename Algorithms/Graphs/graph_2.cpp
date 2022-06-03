#include <iostream>

int main() {
    size_t N, M;
    std::cin >> N >> M;
    auto matr = new bool* [N];
    for (size_t i = 0; i < N; ++i) {
        matr[i] = new bool [N];
        for (size_t j = 0; j < N; ++j)
            matr[i][j] = 0;
    }
    for (size_t i = 0; i < M; ++i) {
        size_t x, y;
        std::cin >> x >> y;
        matr[x][y] = 1;
        matr[y][x] = 1;
    }
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j)
            std::cout << matr[i][j] << ' ';
        std::cout << '\n';
    }
}