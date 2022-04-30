#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned short n;
    scanf("%hu", &n);
//    auto p = new unsigned short* [n];
    auto p = (unsigned short*)malloc(sizeof(unsigned short) * n);
    for (unsigned short i = 0; i < n; ++i) {
        p[i] = 0;
    }
    unsigned short sum = 0;
    for (unsigned short i = 0; i < n; ++i) {
        unsigned short x, y, w;
        scanf("%hu %hu %hu", &x, &y, &w);
        sum += w;
        ++p[x];
    }
    while(1);
//    for (unsigned short i = 0; i < n; ++i) {
//        if (p[i][0])
//            printf("%hu %hu %hu\n", i, p[i][0],p[i][1]);
//        delete [] p[i];
//    }
//    delete [] p;
}