#include <stdio.h>
#include <cmath>

int main() {
    long long x1, y1, x2, y2, x3, y3;
    scanf("%lld %lld %lld %lld %lld %lld", &x1, &y1, &x2, &y2, &x3, &y3);
    double a = sqrt((x2-x3)*(x2-x3) + (y2-y3)*(y2-y3));
    double b = sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3));
    double c = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    double x0 = (a*x1 + b*x2 + c*x3) / (a + b + c);
    double y0 = (a*y1 + b*y2 + c*y3) / (a + b + c);
    printf("%.10lg %.10lg\n", x0, y0);
}