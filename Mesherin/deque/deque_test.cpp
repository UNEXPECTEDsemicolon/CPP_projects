#include "deque.h"
#include <iostream>

struct MyStruct {
    int a;
    int b;

    MyStruct() = delete;

    ~MyStruct() {
//        std::cout << __PRETTY_FUNCTION__ << '\n';
    }

    void operator=(int) {
        throw std::out_of_range("Copying!!!");
    }
};

int main() {
    Deque<int> d;
//    d[0] = 1;
    for (size_t i = 0; i < 334000; ++i) {
        d.push_front(2);
        d.push_front(3);
        d.pop_back();
        d.size();
    }
    auto left = d.begin() + 100'000;
    auto right = d.end() - 233'990;
    while (d.begin() != left) d.pop_front();
    while (d.end() != right) d.pop_back();
    std::cout << d.size() << '\n';
}