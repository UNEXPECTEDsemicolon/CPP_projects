#include <stdio.h>

void swap(long long *x, long long *y) {
    long long t = *x;
    *x = *y;
    *y = t;
}

size_t part(long long *a, size_t l, size_t r) {
    long long d = a[(l+r)/2];
    size_t i=l, j=r;
    for (; i <= j; ) {
        for (; a[i] < d; i++);
        for (; a[j] > d; j--);
        if (i >= j)
            break;
        swap(&a[i++], &a[j--]);
    }
    return j;
}

void qsort(long long *a, size_t l, size_t r) {
    if (l < r) {
        size_t d = part(a, l, r);
        qsort(a, l, d);
        qsort(a, d + 1, r);
    }
}

int main() {
    size_t n, m;
    scanf("%zu %zu", &n, &m);
    long long *a = new long long[n], p;
    for (size_t i=0; i<n; i++)
        scanf("%lld", &a[i]);
    qsort(a, 0, n-1);
    for (size_t i=0; i<m; i++) {
        scanf("%lld", &p);
        for (size_t l=0, r=n-1; l<r;) {
            if (a[l] + a[r] < p)
                l++;
            else if (a[l] + a[r] > p)
                r--;
            else {
                printf("%lld %lld\n", a[l], a[r]);
                break;
            }
            if (l>=r) {
                printf("Not found\n");
                break;
            }
        }
    }
    delete [] a;
}
