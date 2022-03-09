#include "geometry.h"

int main () {
    auto t1 = Triangle(Point(1, 1), Point(7, 3), Point(9, 2));
    auto t2 = Triangle(Point(1, 8), Point(4, 6), Point(2, -3));
    auto t3 = Triangle(Point(0, 1), Point(4, 2), Point(-5, 0));
    auto l1 = t1.EulerLine();
    auto l2 = t2.EulerLine();
    auto l3 = t3.EulerLine();
    std::cout << l1.r0 << ' ' << l1.a << '\n';
    std::cout << l2.r0 << ' ' << l2.a << '\n';
    std::cout << l3.r0 << ' ' << l3.a << '\n';
}
/*
(5.5, -2.5) (0.0370117, 0.999315)
(-0.629032, 2.30645) (0.908778, 0.417279)
(-21.5, 95.5) (0.21857, -0.975821)
 */

