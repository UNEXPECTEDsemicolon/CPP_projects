#include <stdio.h>
#include <cmath>

double sq(long long x0, long long y0, long long x1, long long y1, long long x, long long y) {
    double a, b, c, p;
    a = sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
    b = sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
    c = sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0));
    p = (a + b + c) / 2;
    return sqrt(p*(p-a)*(p-b)*(p-c));
}

int main() {
    size_t n;
    scanf("%zu", &n);
    long long x0, y0;
    scanf("%lld %lld", &x0, &y0);
    long long x1, y1;
    scanf("%lld %lld", &x1, &y1);
    double ans = 0;
    for (size_t i = 2; i < n; ++i) {
        long long x, y;
        scanf("%lld %lld", &x, &y);
        ans += sq(x0, y0, x1, y1, x, y);
        x1 = x;
        y1 = y;
    }
    printf("%.10lg\n", ans);
}