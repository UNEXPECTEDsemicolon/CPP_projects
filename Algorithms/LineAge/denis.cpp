#include <iostream>

int main() {
    int x1, y1, x2, y2, x3, y3;
    std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    if ((x3 - x1) * (x2 - x3) + (y3 - y1) * (y2 - y3) >= 0 and
    (x3 - x1) * (y2 - y3) - (y3 - y1) * (x2 - x3) == 0)
        std::cout << "YES\n" << '\n';
    else
        std::cout << "NO\n";
}