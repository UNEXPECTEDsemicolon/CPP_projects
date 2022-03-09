#include <iostream>
#include <vector>

struct My {
    int a = 0;
};

int main() {
//    auto a = My{2};
    std::vector<int> v(3, 5);
    v[1] = 6;
//    std::cout << (v.begin()->a) << '\n';
}