#include <stdio.h>
#include <stdlib.h>

int main() {
    long long n;
    scanf("%lld\n", &n);
    long long **wins = new long long * [n];
    for (long long i = 0; i < n; i++) {
        wins[i] = new long long[2];
        wins[i][0] = i;
        wins[i][1] = 0;
    }
    for (long long i = 0; i < n; i++) {
        char c;
        for (int j = 0; j < i; j++) {
            scanf("%c", &c);
//            printf("c = <<%c>>\n", c);
            if (c == '+') {
                ++wins[i][1];
//                --wins[j][1];
            }
            else if (c == '-') {
                ++wins[j][1];
//                --wins[i][1];
            }
        }
        scanf("%*[#\n]");
    }
    for (long long i = 0; i < n; ++i) {
        printf("(%lld, %lld) ", wins[i][0], wins[i][1]);
    }
    printf("\n");
    for (long long i = 0; i < n; ++i)
        delete [] wins[i];
    delete [] wins;
}

/*
 * 1: 2
 * 2: 1, 4
 * 3: 1, 4
 * 4: 3
 *
 * 1 2 3 4
 * 3 1 2 4
 * 4 3 1 2
 *
 *
 * 4 3 2 1 1 2 3 2 3 4
 * 4 3 1 2
 * 4 3 1 2 2 3 2 3 4
 *
 * 3 1 1 2 2 4 3 1 2 4
 * 3 1 2 4
 */
//3 4
//2 2
//7 6
//4 5
//
//2 0
//3 1
//4 2
//7 6
// сортировать массив индексов по количеству побед
