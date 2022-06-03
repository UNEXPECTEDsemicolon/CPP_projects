#include <stdio.h>

const size_t BIGNUM = 10010u;

template <typename T, typename T_comp>
class Heap {
public:
    size_t size;
    T *body;
    Heap() {
        body = new T [BIGNUM];
        size = 0;
    }

    Heap(T *a, size_t n) {
        body = new T [BIGNUM];
        size = n;
        for (size_t i = 1; i <= size; i++)
            body[i] = a[i-1];
        for (size_t i = (n + 1) / 2; i >= 1; i--)
            heapify(i);
    }

    ~Heap() {
        while (!isempty())
            pop();
        delete [] body;
    }

    void push(T x) {
        body[++size] = x;
        SiftUp(size);
    }

    T pop() {
        T res = body[1];
        body[1] = body[size--];
        SiftDown(1);
        return res;
    }

    T top() {
        return body[1];
    }

    void replace_top(T x) {
        body[1] = x;
        SiftDown(1);
    }

    bool isempty() {
        return size == 0;
    }

private:
    void swap(T *a, T *b) {
        T t = *a;
        *a = *b;
        *b = t;
    }
    void SiftUp(size_t v) {
        for (; v > 1; v /= 2) {
            if (T_comp()(body[v], body[v / 2]))
                swap(&body[v], &body[v / 2]);
            else
                break;
        }
    }
    void SiftDown(size_t v) {
        while (2*v <= size) {
            T u = 2 * v;
            if (2 * v + 1 <= size && T_comp()(body[2 * v + 1], body[2 * v]))
                u = 2 * v + 1;
            if (T_comp()(body[u], body[v])) {
                swap(&body[u], &body[v]);
                v = u;
            }
            else
                break;
        }
    }
    void heapify(size_t i) {
        size_t l = 2 * i;
        size_t r = 2 * i + 1;
        size_t lar = i;
        if (l <= size && T_comp()(body[l], body[lar]))
            lar = l;
        if (r <= size && T_comp()(body[r], body[lar]))
            lar = r;
        if (lar != i) {
            swap(&body[i], &body[lar]);
            this->heapify(lar);
        }
    }
};

class gt_ll {
public:
    bool operator()(long long x, long long y) {
        return x > y;
    }
};

class lt_ll {
public:
    bool operator()(long long x, long long y) {
        return x < y;
    }
};

void Merge(long long *a, size_t na, long long *b, size_t nb) {
    long long *res = new long long [na + nb];
    size_t i = 0, j = 0;
    for (; i < na && j < nb;) {
        for (; i < na && a[i] <= b[j]; i++)
            res[i+j] = a[i];
        for (; j < nb && a[i] > b[j]; j++) {
            res[i+j] = b[j];
        }
    }
    for (; i < na; i++)
        res[i+j] = a[i];
    for (; j < nb; j++)
        res[i+j] = b[j];
    for (i = 0; i < na + nb; i++)
        a[i] = res[i];
    delete [] res;
}

void MergeSort(long long *a, size_t n) {
    if (n == 1)
        return;
    MergeSort(a, n/2);
    MergeSort(a + n/2, n - n/2);
    Merge(a, n/2, a + n/2, n - n/2);
}

int main() {
    long long mod = 1ll << 30;
    size_t n, k;
    scanf("%zu %zu", &n, &k);
    long long a, x, y;
    scanf("%lld %lld %lld", &a, &x, &y);
    auto h = Heap<long long, gt_ll>();
    for (size_t i = 0; i < k; i++) {
        a = (x * a + y) % mod;
        h.push(a);
    }
    for (size_t i = k; i < n; i++) {
        a = (x * a + y) % mod;
        if (a < h.top()) {
            h.replace_top(a);
        }

    }
    long long *b = new long long [k];
    for (size_t i = 0; i < k; i++)
        b[i] = h.pop();
    MergeSort(b, k);
    for (size_t i = 0; i < k; ++i)
        printf("%lld ", b[i]);
    printf("\n");
    delete [] b;
}//
// Created by max on 10.11.2021.
//

