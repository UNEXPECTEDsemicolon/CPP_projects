#include <iostream>
#include "list2.h"
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
    auto rit = d.cend();
    std::cout << (*--rit) << '\n';
}