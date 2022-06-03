#include <iostream>
#include <vector>
#include <type_traits>

template <typename T, typename = std::enable_if_t<std::is_class<T>::value>>
void f(const T&) {
    std::cout << '1' << '\n';
}

void f(...) {
    std::cout << 2 << '\n';
}

int main() {
    std::string s("abc");
    f(s);
    f(1);
}