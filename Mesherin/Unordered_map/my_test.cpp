#include "unordered_map.h"
#include <unordered_map>
#include <string>

int main() {
    auto m = UnorderedMap<int, std::string>();
    m.emplace(1, "a__");
    m.emplace(2, "b");
    m.emplace(3, "c");
    m.emplace(1, "a__");
    std::cout << m.size() << '\n';
    m.erase(m.begin() + 1, m.end());
    for (auto e: m)
        std::cout << e.first << ' ' << e.second << '\n';
}