#include <stdio.h>
#include <stdlib.h>

int main() {
    const long long BIG = 1000000000ll;
    size_t n;
    scanf("%zu", &n);
    long long *a = new long long [n], x, s=0;
    for (size_t i=0; i<n; i++)
        scanf("%lld", &a[i]);
    size_t i = 0, j = 0;
    for (; i<n; i++) {
        scanf("%lld", &x);
        for (; j<n && x>=a[j]; j++)
            if ((i + j+1) % 2) {
                s = (s + a[j]) % BIG;
            }
        if ((i+1 + j) % 2) {
            s = (s + x) % BIG;
        }
    }
    for (; j<n; j++)
        if ((i + j+1) % 2) {
            s = (s + a[j]) % BIG;
        }
    printf("%lld\n", s);
    delete [] a;
}
