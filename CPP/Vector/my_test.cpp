#include "vector.h"

int main() {
    auto v = Vector<int>(3);
    for (auto x: v) {
        std::cout << typeid(x).name() << '\n';
        std::cout << x << '\n';
    }
    v.PushBack(5);
    v.PushBack(6);
    v.PopBack();
}