#include <stdio.h>

class stack {
public:
    size_t size;
    stack() {
        head = new elem(0);
        size = 0;
    }

    ~stack() {
        while (!empty()) {
            pop();
        }
        delete head;
    }

    void push(size_t val) {
        elem *tmp = new elem(val);
        tmp->prev = head->prev;
        head->prev = tmp;
        size++;
    }

    void pop() {
        elem *tmp = head->prev;
        head->prev = tmp->prev;
//        size_t val = tmp->val;
        delete tmp;
        size--;
//        return val;
    }
#include <stdio.h>

class stack {
public:
    size_t size;
    stack() {
        head = new elem(0);
        size = 0;
    }

    ~stack() {
        while (!empty()) {
            pop();
        }
        delete head;
    }

    void push(size_t val) {
        elem *tmp = new elem(val);
        tmp->prev = head->prev;
        head->prev = tmp;
        size++;
    }

    size_t pop() {
        elem *tmp = head->prev;
        head->prev = tmp->prev;
        size_t val = tmp->val;
        delete tmp;
        size--;
        return val;
    }

    size_t top() {
        return head->prev->val;
    }

    bool empty() {
        return head->prev == nullptr;
    }
private:
    struct elem {
        size_t val;
        elem *prev;
        elem(size_t val) {
            this->val = val;
            this->prev = nullptr;
        }
    };
    elem *head;
};

int main() {
    size_t n;
    scanf("%zu", &n);
    size_t *a = new size_t [n];
    for (size_t i = 0; i < n; i++)
        scanf("%zu", &a[i]);
    size_t *begin = new size_t [n];
    stack *st_b = new stack();
    stack *st_e = new stack();
    for (size_t i = 0; i < n; i++) {
        while (!st_b->empty() && a[st_b->top()] >= a[i])
            st_b->pop();
        begin[i] = st_b->empty() ? 0 : st_b->top() + 1;
        st_b->push(i);
    }
    size_t end, ans = 0;
    size_t sq=  0;
    for (size_t i = n-1; i + 1 >= 1; i--) {
        while(!st_e->empty() && a[st_e->top()] >= a[i])
            st_e->pop();
        end = st_e->empty() ? n + 1 : st_e->top() + 1;
        if (end > begin[i]) {
            sq = (end - begin[i] - 1) * a[i];
        }
        ans = ans < sq ? sq : ans;
        st_e->push(i);
    }
    printf("%zu\n", ans);
    delete [] a;
    delete [] begin;
    delete st_b;
    delete st_e;
}
    size_t top() {
        return head->prev->val;
    }

    bool empty() {
        return head->prev == nullptr;
    }
private:
    struct elem {
        size_t val;
        elem *prev;
        elem(size_t val) {
            this->val = val;
            this->prev = nullptr;
        }
    };
    elem *head;
};

int main() {
    size_t n;
    scanf("%zu", &n);
    size_t *a = new size_t [n];
    for (size_t i = 0; i < n; i++)
        scanf("%zu", &a[i]);
    size_t *begin = new size_t [n];
    stack *st_b = new stack();
    stack *st_e = new stack();
    for (size_t i = 0; i < n; i++) {
        while (!st_b->empty() && a[st_b->top()] >= a[i])
            st_b->pop();
        begin[i] = st_b->empty() ? 0 : st_b->top() + 1;
        st_b->push(i);
    }
    size_t end, ans = 0;
    size_t sq=  0;
    for (size_t i = n-1; i + 1 >= 1; i--) {
        while (!st_e->empty() && a[st_e->top()] >= a[i])
            st_e->pop();
        end = st_e->empty() ? n + 1 : st_e->top() + 1;
        if (end > begin[i]) {
            sq = (end - begin[i] - 1) * a[i];
        }
        ans = ans < sq ? sq : ans;
        st_e->push(i);
    }
    printf("%zu\n", ans);
    delete [] a;
    delete [] begin;
    delete st_b;
    delete st_e;
}