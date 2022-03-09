#include <iostream>
#include <cmath>

const long long N = 1000000ll;

long long f(double x) {
    return (long long)(N*x);
}

bool in(double x, double l, double r) {
    return (f(x) <= f(std::max(l, r)) and f(x) >= f(std::min(l, r)));
}

bool cross(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    if (x1 == x2 and y1 == y2 and x3 == x4 and y3 == y4) {
        if (x1 == x3 and y1 == y3)
            return true;
        else
            return false;
    }
    double a1x, a1y, b1x, b1y, a2x, a2y, b2x, b2y;
    a1x = x1; a1y = y1; b1x = x2-x1; b1y = y2-y1;
    a2x = x3; a2y = y3; b2x = x4-x3; b2y = y4-y3;
    double t1, t2, beta;
    if (x1 == x2 and y1 == y2) {
        if (f(b2x) and f(b2y)) {
            if (f((a1x-a2x)/b2x) == f((a1y-a2y)/b2y)
                and in(x1, x3, x4) and in(y1, y3, y4))
                return true;
            else
                return false;
        }
        else if ((f(b2x) and a1y==a2y or f(b2y) and a1x==a2x)
                 and in(x1, x3, x4) and in(y1, y3, y4))
            return true;
        else
            return false;
    }
    if (x3 == x4 and y3 == y4) {
        if (f(b1x) and f(b1y)) {
            if (f((a2x-a1x)/b1x) == f((a2y-a1y)/b1y)
                and in(x3, x1, x2) and in(y3, y1, y2))
                return true;
            else
                return false;
        }
        else if ((f(b1x) and a1y==a2y or f(b1y) and a1x==a2x)
                 and in(x3, x1, x2) and in(y3, y1, y2))
            return true;
        else
            return false;
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
            return true;
        else
            return false;
    }
    else {
        return false;
    }
    double x0, y0;
    x0 = a2x + beta * b2x; y0 = a2y + beta * b2y;
    if (in(x0, x1, x2) and in(x0, x3, x4) and
        in(y0, y1, y2) and in(y0, y3, y4))
        return true;
    else
        return false;
}

double dist(double x0, double y0, double x1, double y1, double x2, double y2) {
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
    return d;
}

const long long INF = 10000000000ll;
const double EPS = 1e-7;

int main() {
    size_t n = 3;
    auto *x = new long long [n + 1];
    auto *y = new long long [n + 1];
    for (size_t i = 0; i < n; ++i)
        std::cin >> x[i] >> y[i];
    x[n] = x[0];
    y[n] = y[0];
    long long x0, y0;
    std::cin >> x0 >> y0;
    int ans = -1;
    size_t crosses = 0;
    for (size_t i = 1; i < n + 1; ++i) {
        double d = dist(x0, y0, x[i-1], y[i-1], x[i], y[i]);
        if (d < EPS) {
            ans = true;
            break;
        }
        crosses += cross(x0, y0, INF, y0 + 10000, x[i-1], y[i-1], x[i], y[i]);
    }
    if (ans == -1)
        ans = crosses % 2;
    std::cout << (ans ? "In\n" : "Out\n");
}