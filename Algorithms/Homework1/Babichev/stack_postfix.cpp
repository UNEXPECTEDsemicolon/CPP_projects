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

    void push(long long val) {
        elem *tmp = new elem(val);
        tmp->prev = head->prev;
        head->prev = tmp;
        size++;
    }

    long long pop() {
        elem *tmp = head->prev;
        head->prev = tmp->prev;
        long long val = tmp->val;
        delete tmp;
        size--;
        return val;
    }

    long long top() {
        return head->prev->val;
    }

    bool empty() {
        return head->prev == nullptr;
    }
private:
    struct elem {
        long long val;
        elem *prev;
        elem(long long val) {
            this->val = val;
            this->prev = nullptr;
        }
    };
    elem *head;
};

int main() {
    stack *st = new stack;
    char c = '0';
    scanf("%c ", &c);
    while (c != '=') {
        if (c - '0' >= 0 && c - '0' <= 9) {
            st->push(c - '0');
        }
        else {
            long long x, y;
            x = st->pop();
            y = st->pop();
            if (c == '+')
                st->push(y + x);
            else if (c == '-')
                st->push(y - x);
            else if (c == '*')
                st->push(y * x);
        }
        scanf(" %c", &c);
    }
    printf("%lld\n", st->pop());
    delete st;
}
