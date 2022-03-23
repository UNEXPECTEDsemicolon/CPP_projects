#include <iostream>

class C {
    int x = 0;
public:
    C() {}
    C(int x): x(x) {}
    C(const C& source): x(source.x) {}
    ~C() {}
    void swap(C& right) {
        std::swap(x, right.x);
    }
    C& operator=(C source) {
//        auto tmp = source;
        swap(source);
        return *this;
    }
};

int main() {
    auto a = C(1);
    auto b = C(2);
    b = a;
}

