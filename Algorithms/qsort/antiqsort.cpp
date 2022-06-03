#include <stdio.h>

int main() {
    size_t n;
    scanf("%zu", &n);
    long long *a = new long long [n];
    if (n == 1) {
        printf("1\n");
        return;
    }
    if (n % 2 == 1) {
        for (size_t i = 0; i < n / 2 - 1; ++i)
            a[i] = i + 2;
        a[n / 2 - 1] = 1;
        a[n / 2] = n;
        for (size_t i = n / 2 + 1; i < n; ++i)
            a[i] = i;
    }
    else {
        for (size_t i = 0; i < n / 2 - 1; ++i)
            a[i] = n - i - 1;
        a[n / 2 - 1] = n;
        for (size_t i = n / 2; i < n; ++i)
            a[i] = n - i;
    }
    for (size_t i = 0; i < n; i++)
        printf("%lld ", a[i]);
    printf("\n");
    delete [] a;
}