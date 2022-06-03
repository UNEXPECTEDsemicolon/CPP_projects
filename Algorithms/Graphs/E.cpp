#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned short n;
    scanf("%hu", &n);
//    auto p = new unsigned short* [n];
    unsigned short** p = (unsigned short**)malloc(sizeof(unsigned short*) * n);
    for (unsigned short i = 0; i < n; ++i) {
        p[i] = (unsigned short*)malloc(sizeof(unsigned short) * 2);
//        p[i] = new unsigned short [2];
        p[i][0] = p[i][1] = 0;
    }
    for (unsigned short i = 0; i < n; ++i) {
        unsigned short x, y, w;
        scanf("%hu %hu %hu", &x, &y, &w);
        ++p[x][0];
        p[x][1] += w;
    }
//    while(1);
    for (unsigned short i = 0; i < n; ++i) {
        if (p[i][0])
            printf("%hu %hu %hu\n", i, p[i][0],p[i][1]);
        free(p[i]);
    }
    free(p);
}