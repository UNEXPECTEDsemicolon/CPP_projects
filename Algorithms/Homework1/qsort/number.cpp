#include <iostream>
#include <cstring>
#include <stdlib.h>

const size_t m = 110;

int cmp(const void *x, const void *y) {
    char *s1 = *((char **)x);
    char *s2 = *((char **)y);
    for (size_t i = 1; i <= s1[0] && i <= s2[0]; ++i) {
        if (s1[i] > s2[i]) return -1;
        if (s1[i] < s2[i]) return 1;
    }
    for (size_t i = s2[0] + 1; i <= s1[0]; ++i) {
        if (s1[i] > s1[i - s2[0]]) return -1;
        if (s1[i] < s1[i - s2[0]]) return 1;
    }
    for (size_t i = s1[0] + 1; i <= s2[0]; ++i) {
        if (s2[i] > s2[i - s1[0]]) return 1;
        if (s2[i] < s2[i - s1[0]]) return -1;
    }
    auto *sum1 = new char [s1[0] + s2[0]];
    auto *sum2 = new char [s1[0] + s2[0]];
    memcpy(sum1, s1 + 1, s1[0]);
    memcpy(sum1 + s1[0], s2 + 1, s2[0]);
    memcpy(sum2, s2 + 1, s2[0]);
    memcpy(sum2 + s2[0], s1 + 1, s1[0]);
    int ans = 0;
    for (size_t i = 0; i < s1[0] + s2[0]; ++i) {
        if (sum1[i] > sum2[i]) {
            ans = -1;
            break;
        }
        if (sum1[i] < sum2[i]) {
            ans = 1;
            break;
        }
    }
    delete [] sum1;
    delete [] sum2;
    return ans;
}

int main() {
    auto **s = new char * [m];
    size_t n = 0;
    auto *cur_s = new char [m];
    for (; std::cin >> cur_s && cur_s[0] != '#'; ++n) {
        s[n] = new char [m + 1];
        strcpy(s[n] + 1, cur_s);
        s[n][0] = strlen(cur_s);
    }
    delete [] cur_s;
    qsort(s, n, sizeof(char *), cmp);
    for (size_t i = 0; i < n; ++i) {
        std::cout << (s[i] + 1);
        delete[] s[i];
    }
    std::cout << '\n';
    delete [] s;
}