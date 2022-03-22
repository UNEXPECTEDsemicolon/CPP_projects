#include <iostream>
#include "list.h"
#include <deque>
#include <vector>
//#include <list>

int main() {
    const int N = 1000;
    StackStorage<N> storage;
    StackAllocator<int, N> alloc(storage);
    List<int, StackAllocator<int, N>> d(alloc);
    d.push_back(5);
    d.push_front(6);
    d.pop_back();
    d.push_front(7);
    auto begin = d.begin();
    std::cout << std::is_assignable< decltype(begin), decltype(d.cbegin())>::value << '\n';
    begin = d.cbegin();
    std::cout << (*(d.begin())) << '\n';
}