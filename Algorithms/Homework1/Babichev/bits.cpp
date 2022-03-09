#include <stdio.h>
#include <time.h>

typedef long long (*func) (unsigned);
void time_of(const char *name, func f, unsigned lim) {
    clock_t start = clock();
    long long res = f(lim);
    clock_t end = clock();
    printf("[%s]: f(%u) = %.3lf res=%lld\n",
           name, lim, (double)(end - start)/CLOCKS_PER_SEC, res);
}

long long func1(unsigned lim) {
    long long res = 0;
    for (unsigned x = 0; x < lim; x += 1) {
        // сколько битов в x?
        for (unsigned i = 0; i < 32; i++) {
            if ((x >> i) & 1) res++;
        }
    }
    return res;
}

long long func2(unsigned lim) {
    long long res = 0;
    for (unsigned x = 0; x < lim; x += 1) {
        // сколько битов в x?
        for (unsigned i = 0; i < 32; i++) {
            res += (x >> i) & 1;
        }
    }
    return res;
}

long long func3(unsigned lim) {
    long long res = 0;
    for (unsigned x = 0; x < lim; x += 1) {
        // сколько битов в x?
        for (unsigned i = x; i != 0; i >>= 1) {
            res += i & 1;
        }
    }
    return res;
}

long long func4(unsigned lim) {
    long long res = 0;
    for (unsigned x = 0; x < lim; x += 1) {
        // сколько битов в x?
        for (unsigned i = x; i != 0; i &= (i-1)) {
            res++;
        }
    }
    return res;
}

long long func5(unsigned lim) {
    long long res = 0;
    for (unsigned x = 0; x < lim; x += 1) {
        // сколько битов в x?
        unsigned i = x;
        i = (i & 0x55555555) + ((i >> 1)  & 0x55555555);
        i = (i & 0x33333333) + ((i >> 2)  & 0x33333333);
        i = (i & 0x0F0F0F0F) + ((i >> 4)  & 0x0F0F0F0F);
        i = (i & 0x00FF00FF) + ((i >> 8)  & 0x00FF00FF);
        i = (i & 0x0000FFFF) + ((i >> 16) & 0x0000FFFF);
        res += i;
    }
    return res;
}


int main() {
    enum {TRIES = 100000000};
    time_of("func1", func1, TRIES);
    time_of("func2", func2, TRIES);
    time_of("func3", func3, TRIES);
    time_of("func4", func4, TRIES);
    time_of("func5", func5, TRIES);
}