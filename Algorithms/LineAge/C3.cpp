#include <iostream>

const long long N = 1000000;

long long f(double x) {
    return (long long)(N*x);
}

bool in(double x, double l, double r) {
    return (f(x) <= f(std::max(l, r)) and f(x) >= f(std::min(l, r)));
}

int main() {
    double x1, y1, x2, y2, x3, y3, x4, y4;
    std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
    if (x1 == x2 and y1 == y2 and x3 == x4 and y3 == y4) {
        if (x1 == x3 and y1 == y3)
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
        return 0;
    }
    double a1x, a1y, b1x, b1y, a2x, a2y, b2x, b2y;
    a1x = x1; a1y = y1; b1x = x2-x1; b1y = y2-y1;
    a2x = x3; a2y = y3; b2x = x4-x3; b2y = y4-y3;
    double t1, t2, beta;
    if (x1 == x2 and y1 == y2) {
        if (f(b2x) and f(b2y)) {
            if (f((a1x-a2x)/b2x) == f((a1y-a2y)/b2y)
                and in(x1, x3, x4) and in(y1, y3, y4))
                std::cout << "YES\n";
            else
                std::cout << "NO\n";
        }
        else if ((f(b2x) and a1y==a2y or f(b2y) and a1x==a2x)
                 and in(x1, x3, x4) and in(y1, y3, y4))
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
        return 0;
    }
    if (x3 == x4 and y3 == y4) {
        if (f(b1x) and f(b1y)) {
            if (f((a2x-a1x)/b1x) == f((a2y-a1y)/b1y)
                and in(x3, x1, x2) and in(y3, y1, y2))
                std::cout << "YES\n";
            else
                std::cout << "NO\n";
        }
        else if ((f(b1x) and a1y==a2y or f(b1y) and a1x==a2x)
                 and in(x3, x1, x2) and in(y3, y1, y2))
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
        return 0;
    }
    if (f(b1x)) {
        t1 = b1y*b2x/b1x - b2y;
        t2 = a2y - a1y + b1y*(a1x-a2x)/b1x;
    }
    else {
        t1 = b1x*b2y/b1y - b2x;
        t2 = a2x - a1x + b1x*(a1y-a2y)/b1y;
    }
    if (f(t1))
        beta = t2 / t1;
    else if (f(t1)==0 and f(t2)==0) {
        if (in(x1, x3, x4) and in(y1, y3, y4) or
                               in(x2, x3, x4) and in(y2, y3, y4) or
                                                  in(x3, x1, x2) and in(y3, y1, y2) or
                                                                     in(x4, x1, x2) and in(y4, y1, y2))
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
        return 0;
    }
    else {
        std::cout << "NO";
        return 0;
    }
    double x0, y0;
    x0 = a2x + beta * b2x; y0 = a2y + beta * b2y;
    if (in(x0, x1, x2) and in(x0, x3, x4) and
        in(y0, y1, y2) and in(y0, y3, y4))
        std::cout << "YES\n";
    else
        std::cout << "NO\n";
}


/*
 *
            std::cout << x[i-1] << ' ' << y[i-1] << ' ' << x[i] << ' ' << y[i] << ' ' << crosses << '\n';

 */