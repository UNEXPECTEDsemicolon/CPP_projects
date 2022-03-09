/*
 * max heap
 * min heap
 * медиану хранить отдельно
 */

#include <stdio.h>

const long long BIGNUM = 1000000ll;

template <typename T, typename T_comp>
class Heap {
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
        SiftDown(1);
        return res;
    }

    T top() {
        return body[1];
    }

    bool isempty() {
        return size == 0;
    }

private:
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
        while (2*v < size) {
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
    Heap<long long, gt_ll> maxheap = Heap<long long, gt_ll>();
    Heap<long long, lt_ll> minheap = Heap<long long, lt_ll>();
    long long med = -10000000000ll;
    size_t n;
    scanf("%zu", &n);
    for (size_t i = 0; i < n; i++) {
        int act;
        scanf("%d", &act);
        if (act == 0) {
            long long x;
            scanf("%lld", &x);
            if (i == 0) {
                med = x;
                continue;
            }
            if (x >= med)
                minheap.push(x);
            else
                maxheap.push(x);
            if (minheap.size >= 2 + maxheap.size) {
                maxheap.push(med);
                med = minheap.top();
                minheap.pop();
            }
            else if (maxheap.size >= 1 + minheap.size) {
                minheap.push(med);
                med = maxheap.top();
                maxheap.pop();
            }
        }
        else
            printf("%lld\n", med);
    }
}