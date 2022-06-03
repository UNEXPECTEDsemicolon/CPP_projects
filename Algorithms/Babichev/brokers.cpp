#include <stdio.h>

const long long BIGNUM = 1000000ll;

template <typename T, typename T_comp>
struct Heap {
public:
    size_t size;
    Heap() {
        body = new T [BIGNUM];
        size = 0;
    }

    Heap(T* const a, size_t n) {
        body = new T [BIGNUM];
        size = n;
        for (size_t i = 0; i < n; i++)
            body[i+1] = a[i];
        for (size_t i = n; i >= 1; i--)
            SiftUp(i);
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
//        body[1] = (T)(1ull << (sizeof(T) * 8 - 2));
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

//private:
    T *body;
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
            size_t u = 2 * v;
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


int main() {
    Heap<long long, lt_ll> h;
    size_t n;
    scanf("%zu", &n);
    for (size_t i = 0; i < n; ++i) {
        long long x;
        scanf("%lld", &x);
        h.push(x);
    }
    long long ans = 0;
    while (h.size != 1) {
        long long cur_sum = h.pop() + h.pop();
        h.push(cur_sum);
        ans += cur_sum;
    }
    printf("%lld.%02lld\n", ans / 100, ans % 100);
}