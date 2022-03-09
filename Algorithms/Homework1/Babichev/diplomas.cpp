// f(k) = max(k*w, ceil(n/k)*h) -> min

#include <stdio.h>

long long max(long long x, long long y) {
    return x >= y ? x : y;
}
long long min(long long x, long long y) {
    return x >= y ? y : x;
}

long long f(long long k, long long w, long long h, long long n) {
    return max(k * w, (n + k-1) / k * h);
}

long long binsearch(long long k, long long w, long long h, long long n) {
    long long l = 1, r = n;
    while (r - l > 1) {
        if (f(k, w, h, n) < f(k + 1, w, h, n))
            r = k;
        else
            l = k;
        k = (r + l) / 2;
    }
    k = f(l, w, h, n) < f(r, w, h, n) ? l : r;
    return k;
}

int main() {
    long long w, h, n;
    scanf("%lld %lld %lld", &w, &h, &n);
    long long k1, k2, ans1, ans2;
    k1 = binsearch(1, w, h, n);
    k2 = binsearch(n, w, h, n);
    ans1 = max(k1*w, (n + k1-1)/k1*h);
    ans2 = max(k2*w, (n + k2-1)/k2*h);
    printf("%lld", min(ans1, ans2));
}
