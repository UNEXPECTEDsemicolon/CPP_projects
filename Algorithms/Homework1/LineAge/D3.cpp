#include <iostream>
int main() {
    int x1, y1, x2, y2, x3, y3;
    std::cin >> x1 >>y1 >> x2 >> y2 >> x3 >> y3;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(10);
    std::cout << (x1 + x2 + x3) / 3.0 << ' ' << (y1 + y2 +y3) / 3.0 << '\n';
}