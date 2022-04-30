#include "matrix.h"

template <size_t N>
auto dot(Matrix<N, 1>& x, Matrix<N, 1>& y) {
    auto G = Matrix<N, N>({{2, 0, -1}, {0, 2, 1}, {-1, 1, 2}});
    return (x.transposed() * G * y)[0][0];
}

int main() {
    auto A = Matrix<3, 2>({{2, -3}, {1, 4}, {-5, 0}});
    auto B = Matrix<2, 3>({{4, -3, 0}, {6, -2, 2}});
    auto C = Matrix<3, 3>({{1, 2, -3}, {4, 5, 5}, {0, -3, 7}});
//    std::cout << A * B;
    std::cout << A * B * Rational(3) - C.transposed() * Rational(2);
//    auto r = Rational(BigInteger("46513135154653413543513524351435435"), BigInteger("435138943514641684640613406446464264"));
//    std::cout << r * r * r * r * r << '\n';
}
