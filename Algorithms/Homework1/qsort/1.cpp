#include <stdio.h>
#include <stdlib.h>

int cmp_int(const void * p1, const void * p2) {
    return *(const int*)p1 - *(const int*)p2;
}

int main() {
    int n;
    scanf("%d", &n);
    int *a = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    int *ev = (int *)malloc(sizeof(int) * n);
    int *inds = (int *)malloc(sizeof(int) * n);
    int L = 0;
    for (int i = 0; i < n; i++)
        if (a[i] % 2 == 0) {
            ev[L] = a[i];
            inds[L] = i;
            L++;
        }
    qsort(ev, L, sizeof(int), cmp_int);
    for (int i = 0, j = 0; i < n; i++) {
        if (j < L && i == inds[j]) {
            printf("%d ", ev[j]);
            j++;
        }
        else
            printf("%d ", a[i]);
    }
    free(a);
    free(ev);
    free(inds);
}