#include <iostream>

int main() {
    double x1, y1, x2, y2, x3, y3;
    std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    double denum = x1 * y2 + y1 * x3 + x2 * y3 - y2 * x3 - x1 * y3 - y1 * x2;
    double x0 = (y1 * (x3 * x1 + y2 * y2) + y3 * (x2 * x3 + y1 * y1) + y2 * (x1 * x2 + y3 * y3)
                 - y3 * (x3 * x1 + y2 * y2) - y1 * (x1 * x2 + y3 * y3) - y2 * (x2 * x3 + y1 * y1)) / denum;
    double y0 = (x2 * (x1 * x1 + y2 * y3) + x1 * (x3 * x3 + y1 * y2) + x3 * (x2 * x2 + y3 * y1)
                 - x2 * (x3 * x3 + y1 * y2) - x3 * (x1 * x1 + y2 * y3) - x1 * (x2 * x2 + y3 * y1)) / denum;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(10);
    std::cout << x0 << ' ' << y0 << '\n';
}