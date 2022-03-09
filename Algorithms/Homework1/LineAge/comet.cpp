#include <iostream>
#include <cmath>

int main() {
    double xa, ya, za, xb, yb, zb;
    std::cin >> xa >> ya >> za >> xb >> yb >> zb;
    double vax, vay, vaz, vbx, vby, vbz;
    std::cin >> vax >> vay >> vaz >> vbx >> vby >> vbz;
    double R;
    std::cin >> R;
    double a, b, c;
    a = (vbx - vax) * (vbx - vax) + (vby - vay) * (vby - vay) + (vbz - vaz) * (vbz - vaz);
    b = (xb - xa) * (vbx - vax) + (yb - ya) * (vby - vay) + (zb - za) * (vbz - vaz);
    c = (xb - xa) * (xb - xa) + (yb - ya) * (yb - ya) + (zb - za) * (zb - za) - R * R;
    double t0 = -b / a;
    if (a == 0.0 || a * t0 * t0 + 2 * b * t0 + c >= 0.0) {
        std::cout << "-1\n";
    } else {
        double t1 = std::max((-b - sqrt(b * b - a * c)) / a, 0.0);
        double t2 = std::max((-b + sqrt(b * b - a * c)) / a, 0.0);
        if (t1 == t2)
            std::cout << "-1\n";
        else {
            std::cout.setf(std::ios::fixed);
            std::cout.precision(15);
            std::cout << t1 << ' ' << t2 << '\n';
        }
    }
}

/*

0 0 0
1 1 0
0 0 0
-1 1 0
1

 */