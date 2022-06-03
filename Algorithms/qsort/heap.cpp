#include <stdio.h>

const size_t BIGNUM = 1000010ull;

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

    T *push(T x) {
        body[++size] = x;
        return SiftUp(size);
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

    void decrease_key(T *key, T delta) {
        *key -= delta;
        SiftUp(key - body);
    }

private:
    void swap(T *a, T *b) {
        T t = *a;
        *a = *b;
        *b = t;
    }
    T *SiftUp(size_t v) {
        T *res = &body[v];
        for (; v > 1; v /= 2) {
            if (T_comp()(body[v], body[v / 2])) {
                swap(&body[v], &body[v / 2]);
            }
            else
                break;
        }
        return res;
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

int main() {
    auto h = Heap<long long, lt_ll>();
    size_t q;
    scanf("%zu\n", &q);
    auto **history = new long long * [q];
    char act;
    for (size_t i = 0; i < q; i++) {
        scanf("%c", &act);
        if (act == 'i') {
            scanf("%*6s");
            long long x;
            scanf("%lld", &x);
            history[i] = h.push(x);
        } else if (act == 'g') {
            scanf("%*s");
            printf("%lld\n", h.top());
        } else if (act == 'e') {
            scanf("%*s");
            h.pop();
        } else if (act == 'd') {
            scanf("%*11s");
            size_t I;
            long long D;
            scanf("%zu %lld", &I, &D);
            h.decrease_key(history[I-1], D);
        }
        scanf("%*[\n]");
    }
    delete [] history;
}