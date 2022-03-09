#include <stdio.h>

class stack_ll {
public:
    using T = char;
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

int main() {
    char x;
    stack_ll *stack = new stack_ll();
    scanf("%c", &x);
    while (x == '[' || x == ']' ||
           x == '{' || x == '}' ||
           x == '(' || x == ')' ||
           x == '<' || x == '>') {
        if (x == '{' || x == '[' || x == '(' || x == '<') {
            stack->push(x);
        }
        else if (!(stack->empty()) && x == stack->top() + (x == ')' ? 1 : 2))
            stack->pop();
        else {
            printf("NO\n");
            delete stack;
            return 0;
        }
        scanf("%c", &x);
    }
    printf(stack->empty() ? "YES\n" : "NO\n");
    delete stack;
}