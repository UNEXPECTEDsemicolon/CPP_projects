#include <iostream>
#include <cmath>

int main() {
    double x0, y0, x1, y1, x2, y2;
    std::cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
    double cos1 = ((x0 - x1)*(x2-x1) + (y0-y1)*(y2-y1)) /
            (sqrt((x0 - x1)*(x0 - x1) + (y0-y1)*(y0-y1)) * sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
    double cos2 = ((x0 - x2)*(x1-x2) + (y0-y2)*(y1-y2)) /
            (sqrt((x0 - x2)*(x0 - x2) + (y0-y2)*(y0-y2)) * sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
    double d;
    if (cos1 >= 0 && cos2 >= 0) {
        double a, b, c;
        a = y1 - y2;
        b = x1 - x2;
        c = x1*y2 - x2*y1;
        d = std::abs(a * x0 - b * y0 + c) /
            sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
    }
    else
        d = std::min(sqrt(pow(x1-x0, 2) + pow(y1-y0, 2)),
                     sqrt(pow(x2-x0, 2) + pow(y2-y0, 2)));
    std::cout.setf(std::ios::fixed);
    std::cout.precision(10);
    std::cout << d << '\n';
}