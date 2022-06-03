#include <stdio.h>

long long max(long long x, long long y) {
    return (x >= y) ? x : y;
}

void swap(long long *x, long long *y) {
    long long t = *x;
    *x = *y;
    *y = t;
}

void Merge(long long *begin_a, long long *end_a, long long *begin_b, long long *end_b) {
    for (long long *i = begin_a; i <= end_a; i++)
        if (*i > *begin_b)
            swap(i, begin_b);
    for (long long *i = begin_b; i < end_b; i++)
        if (*i > *(i + 1))
            swap(i, i+1);
}

void MergeSort(long long *begin, long long *end) {
    long long n = end - begin + 1;
    if (n == 1)
        return;
    MergeSort(begin, begin + n/2 - 1);
    MergeSort(begin + n/2, end);
    Merge(begin, begin + n/2 - 1, begin + n/2, end);
}

int main() {
    size_t n;
    scanf("%zu", &n);
    long long *h = new long long [n], *h_copy = new long long [n];
    for (size_t i = 0; i < n; i++) {
        scanf("%lld", &h[i]);
        h_copy[i] = h[i];
    }
    MergeSort(h_copy, h_copy + n - 1);
    long long ans = 0;
    for (size_t i = 0; i < n; i++) {
        if (i != 0 && h_copy[i] == h_copy[i-1])
            continue;
        ans = max(ans, h_copy[i]);
        long long len_line = 0;
        for (size_t j = 0; j < n; j++) {
            if (h[j] >= h_copy[i])
                len_line++;
            else {
                ans = max(ans, len_line * h_copy[i]);
                len_line = 0;
            }
        }
        ans = max(ans, len_line * h_copy[i]);
    }
    printf("%lld\n", ans);
    delete [] h;
    delete [] h_copy;
}