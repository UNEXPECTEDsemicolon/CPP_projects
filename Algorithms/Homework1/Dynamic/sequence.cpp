#include <stdio.h>

int main() {
    size_t n;
    scanf("%zu", &n);
    auto F = new size_t [n + 3];
    F[1] = F[2] = 1;
    for (size_t i = 3; i <= n + 2; ++i)
        F[i] = F[i-1] + F[i-2];
    printf("%zu\n", F[n+2]);
    delete [] F;
}