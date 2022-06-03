#include <iostream>
//#include "deque_test.h"

class A {
private:
    int x = 0;
public:
    class B {
    public:
        A* origin;
        B(A* origin): origin(origin) {}
        void print() {
            std::cout << origin->x << '\n';
        }
    };
    A(int x): x(x) {};
};

int main() {
    auto a = A(5);
    typename A::B u(&a);
    u.print();
}