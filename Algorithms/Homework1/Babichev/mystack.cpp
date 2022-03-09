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
    char *s = new char [6];
    while (true) {
        scanf("%s", s);
        if (s[0] == 'p') {
            if (s[1] == 'u') {
                long long x;
                scanf("%lld", &x);
                st->push(x);
                printf("ok\n");
            }
            else {
                if (!st->empty())
                    printf("%lld\n", st->pop());
                else
                    printf("error\n");
            }
        }
        else if (s[0] == 'b') {
            if (!st->empty())
                printf("%lld\n", st->top());
            else
                printf("error\n");
        }
        else if (s[0] == 's') {
            printf("%zu\n", st->size);
        }
        else if (s[0] == 'c') {
            while (!st->empty())
                st->pop();
            printf("ok\n");
        }
        else if (s[0] == 'e') {
            printf("bye\n");
            break;
        }
        scanf("\n");
    }
    delete [] s;
    delete st;
}
