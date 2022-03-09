#include "matrix.h"

int main() {
    const size_t m = 20, n = 20;
//    vector<vector<int>> vec(m, vector<int>(n, 0));
//    for (size_t i = 0; i < m; ++i) {
//        for (size_t j = 0; j < n; ++j) {
//            std::cin >> vec[i][j];
//        }
//    }
//    auto matr = Matrix<m, n>(vec);
    auto matr = SquareMatrix<n>();
//    std::cout << matr.inverted();
    auto det = matr.det();
    matr[0][0] /= det;
    while(1);
    std::cout << matr[0][0];
//    matr *= matr;
//    auto temp = &matr[0];
//    std::cout << sizeof(temp) << '\n';
//    std::cout << temp[0] << '\n';
//    std::cout << "Hi\n";
//    while(1);
//    std::cout << matr * matr << '\n';
}

/*

-6 10
-8 2

11 10
9 2

1 4
9 2



 */