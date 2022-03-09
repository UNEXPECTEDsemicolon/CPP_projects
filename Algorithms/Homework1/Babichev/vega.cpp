#include <stdio.h>

class stack_ll {
public:
    using T = long long;
    stack_ll() {
        head = new node(0);
    }

    ~stack_ll() {
        while (!empty()) {
            pop();
        }
        delete head;
    }

    void push(T val) {
        node *tmp = new node(val);
        tmp->prev = head->prev;
        head->prev = tmp;
    }

    T pop() {
        node *tmp = head->prev;
        head->prev = tmp->prev;
        T val = tmp->val;
        delete tmp;
        return val;
    }

    T top() {
        return head->prev->val;
    }

    bool empty() {
        return head->prev == nullptr;
    }
private:
    struct node {
        T val;
        node *prev;
        node(T val) {
            this->val = val;
            this->prev = nullptr;
        }
    };
    node *head;
};

class queue {
    using T = long long;
public:
    T size;
    queue() {
        size = 0;
        add = new stack_ll();
        del = new stack_ll();
    }

    ~queue() {
        while (!add->empty())
            add->pop();
        delete add;
        while (!del->empty())
            del->pop();
        delete del;
    }

    void push(T x) {
        add->push(x);
        size++;
    }

    void push_on_top(T x) {
        del->push(x);
        size++;
    }

    T front() {
        if (del->empty())
            while (!add->empty())
                del->push(add->pop());
        return del->top();
    }

    T pop() {
        front();
        size--;
        return del->pop();
    }

private:
    stack_ll *add, *del;
};

int main() {
    queue q_part1, q_part2;
    long long n;
    scanf("%lld\n", &n);
    for (long long j=0; j<n; j++) {
        char sign;
        long long i;
        scanf("\n%c", &sign);
        if (sign == '+') {
            scanf("%lld", &i);
            q_part2.push(i);
        }
        else if (sign == '*') {
            scanf("%lld", &i);
            q_part2.push_on_top(i);
        }
        else {
            printf("%lld\n", q_part1.pop());
        }
        if (q_part1.size - q_part2.size < 0) {
            long long t = q_part2.pop();
            q_part1.push(t);
        }
    }
}
