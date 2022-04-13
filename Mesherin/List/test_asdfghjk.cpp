#include <iostream>

class A {
public:
    A() = delete;
    A(int x) {}
};

class C {
public:
    A x;
    C(int y) {

    };
};

int main() {

}