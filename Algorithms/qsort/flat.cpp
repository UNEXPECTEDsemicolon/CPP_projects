//test 9
/*
1 P1230023459
2 D09009090009
3 E00000222339939399393
4 E99999999
5 D99999999
6 P99999999
7 D998298200001909
8 D123894998
9 P898373788787


1 P1230023459
2 D9009090009
3 E222339939399393
4 E99999999
5 D99999999
6 P99999999
7 D998298200001909
8 D123894998
9 P898373788787
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

const size_t n = 10010;
const size_t m = 510;

struct money {
    char *val = nullptr;
    char cur;
    size_t len = 0;
    size_t num;

    money(size_t flat_num, char cur, const char *val): num(flat_num), cur(cur) {
        for (; len < m + 1 && val[len] != '\0'; len++);
        this->val = new char [len];
        for (size_t j = 0; j < len; j++)
            this->val[j] = val[j];
    }

    ~money(){
        delete [] val;
    }

    bool exist() {
        return (val == nullptr);
    }
};

int cmp_money(const void * p1, const void * p2) {
    const money *m1 = *((const money **)p1);
    const money *m2 = *((const money **)p2);
    if (m1->cur < m2->cur)
        return -1;
    if (m1->cur > m2->cur)
        return 1;
    if (m1->len < m2->len)
        return 1;
    if (m1->len > m2->len)
        return -1;
    for (size_t i = 0; i < m1->len; i++) {
        if (m1->val[i] < m2->val[i])
            return 1;
        if (m1->val[i] > m2->val[i])
            return -1;
    }
    return 0;
}

int main() {
    money **a = new money* [n];
    size_t real_n = 0;
    char *cur_and_val = new char [m];
    for (size_t j = 0; j < m; j++)
        cur_and_val[j] = '\0';
    size_t flat_num;
    for (; std::cin >> flat_num >> cur_and_val; real_n++) {
        char cur = cur_and_val[0];
        std::cout << "cur = " << cur << '\n';
        char *begin = cur_and_val + 1;
        for (; *begin == '0'; ++begin);
        if (*begin == '\0') --begin;
        a[real_n] = new money(flat_num, cur, begin);
    }
    delete [] cur_and_val;
    qsort(a, real_n, sizeof(money*), cmp_money);
    for (size_t i = 0; i < real_n; i++)
        printf("%zu ", a[i]->num);
    printf("\n");
    for (size_t i = 0; i < real_n; i++)
        delete a[i];
    delete [] a;
}