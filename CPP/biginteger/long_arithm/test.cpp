#include <stdio.h>

int main() {
    size_t s = 1ull << (sizeof(size_t) / 2 * 8);
    printf("%zu", s);
}