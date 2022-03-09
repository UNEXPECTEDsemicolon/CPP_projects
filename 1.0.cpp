#include <stdio.h>

long long calc_sum(size_t k, size_t *n, long long **a, bool *used, size_t start = 0) {
    if (start == k) return 1;
    long long sum = 0;
    for (size_t i = 0; i < n[start]; i++) {
        if (!used[i]) {
            used[i] = true;
            sum += calc_sum(k, n, a, used, start + 1) * a[start][i];
            used[i] = false;
        }
    }
    return sum;
}

int main(int argc, char **argv) {
    size_t k = 0 * argc; // anti_warning :)
    size_t real_n_len = 1;
    size_t *n = new size_t [real_n_len];
    for (size_t i = 1; argv[i] != nullptr; i++) {
        if (++k > real_n_len) {
            size_t *temp = new size_t [real_n_len * 2];
            for (size_t j = 0; j < real_n_len; ++j)
                temp[j] = n[j];
            delete [] n;
            n = temp;
            real_n_len *= 2;
        }
        sscanf(argv[i], "%zu", &n[i - 1]);
    }
    auto **a = new long long* [k];
    for (size_t i = 0; i < k; i++) {
        a[i] = new long long [n[i]];
        for (size_t j = 0; j < n[i]; j++) {
            int trash = scanf("%lld", &a[i][j]);
            ++trash; // anti_warning :)
        }
    }
    size_t max_n = 0;
    for (size_t i = 0; i < k; ++i)
        if (n[i] > max_n)
            max_n = n[i];
    auto *used = new bool [max_n];
    for (size_t i = 0; i < max_n; ++i) used[i] = false;
    printf("%lld\n", calc_sum(k, n, a, used));
    delete [] n;
    delete [] used;
    for (size_t i = 0; i < k; i++)
        delete [] a[i];
    delete [] a;
}
