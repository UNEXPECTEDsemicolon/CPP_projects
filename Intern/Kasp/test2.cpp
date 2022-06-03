#include <iostream>
#include <string>

int main() {
    std::string s;
    std::cin >> s;
    size_t count = 0;
    for (auto c: s) {
        if (c == '(')
            ++count;
        else if (c == ')') {
            if (count > 0)
                --count;
            else {
                std::cout << 0 << '\n';
                return 0;
            }
        }
    }
    std::cout << (count == 0) << '\n';
}