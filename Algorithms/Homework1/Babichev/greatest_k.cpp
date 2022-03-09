#include <stdio.h>
#include <string>
#include <iostream>

const size_t BIGNUM = 10010u;

template <typename T>
class Heap {
public:
    size_t size;
    T *body;
    Heap()
        : body(new T [BIGNUM]), size(0) {}

    Heap(T *a, size_t n)
        : body(new T [BIGNUM]), size(n) {
        for (size_t i = 1; i <= size; i++)
            body[i] = a[i-1];
        for (size_t i = (n + 1) / 2; i >= 1; i--)
            heapify(i);
    }

    ~Heap() {
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

    bool isempty() {
        return size == 0;
    }

    void replace_top(T x) {
        body[1] = x;
        SiftDown(1);
    }

private:
    void swap(T *a, T *b) {
        T t = *a;
        *a = *b;
        *b = t;
    }
    void SiftUp(size_t v) {
        for (; v > 1; v /= 2) {
            if (body[v] < body[v / 2])
                swap(&body[v], &body[v / 2]);
            else
                break;
        }
    }
    void SiftDown(size_t v) {
        while (2*v <= size) {
            T u = 2 * v;
            if (2 * v + 1 <= size && body[2 * v + 1] < body[2 * v])
                u = 2 * v + 1;
            if (body[u] < body[v]) {
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
        if (l <= size && body[l] < body[lar])
            lar = l;
        if (r <= size && body[r] < body[lar])
            lar = r;
        if (lar != i) {
            swap(&body[i], &body[lar]);
            this->heapify(lar);
        }
    }
};

bool read_int_unlocked(long long & out) {
    int c = getchar_unlocked();
    long long x = 0;
    int neg = 0;

    for (; !('0'<=c && c<='9') && c != '-'; c = getchar_unlocked()) {
        if (c == EOF)
            return false;
    }
    if (c == '-') {
        neg = 1;
        c = getchar_unlocked();
    }
    if (c == EOF)
        return false;
    for (; '0' <= c && c <= '9' ; c = getchar_unlocked())
        x = x*10 + c - '0';
    out = neg ? -x : x;
    return true;
}

int main() {
    size_t n, k;
    scanf("%zu %zu", &n, &k);
    auto h = Heap<long long>();
    long long x;
    for (size_t i = 0; i < k; ++i) {
        read_int_unlocked(x);
        h.push(x);
    }
    for (size_t i = k; i < n; i++) {
        read_int_unlocked(x);
        if (x > h.top())
            h.replace_top(x);
    }
    auto *sorted_k = new long long [k];
    for (size_t i = 0; i < k; i++)
        sorted_k[k - i - 1] = h.pop();
    for (size_t i = 0; i < k; i++)
        printf("%lld\n", sorted_k[i]);
    delete [] sorted_k;
}