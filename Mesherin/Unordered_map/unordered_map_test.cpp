#include "unordered_map.h"
#include <unordered_map>
#include <string>

struct S {
    int x;
    S() = delete;
    S(const S&) = delete;
//    S(const S&) = default;
    S(S&&) = default;
    S(const S&& src) {
        x = std::move(src.x);
    }
    S(int x): x(x) {}

    template<typename T>
    int operator()(const T&) {
        return 0;
    }
};

bool operator==(const S& left, const S& right) {
    return left.x == right.x;
}

template<typename T>
auto f(T&& p) {
//    std::pair<S, S> p2 = {const_cast<std::remove_const_t<decltype(std::move(p.first))>>(std::move(p.first)), std::move(p.second)};
//    return p2;
}


struct MyHash {
    size_t operator()(const S& val) const noexcept {
        return std::hash<int>()(val.x);
    }
};

int main() {
//    auto h = UnorderedMap<std::string, int>();
//    h.insert({"a", 1});
//    h.insert({"b", 2});
//    h.insert({"c", 3});
//    for (int i = 0; i < 100; ++i) {
//        h.insert({i, "abc"});
//    }
//    std::cout << h.size() << ' ' << h.bucket_count << '\n';
//    std::cout << h.load_factor() << '\n';
//    h[6] = 2;
//    h.reserve(20);
//    for (auto it = h.begin(); it != h.end(); ++it)
//        std::cout << it->second << ' ';
//    std::cout << '\n';
//    std::cout << (h.at(7)) << '\n';
//    std::cout << h.size() << '\n';
    std::pair<S, S> p1 = {1, 2};
    std::pair<const S, S> p2 = std::move(p1);
    std::cout << p2.first.x << '\n';
//    std::cout << f(std::move(p1)).first.x << '\n';
//    auto p2 = std::move(p1);
//    std::cout << p2.first.x << '\n';
//    const auto p1 = P(1);
//    auto p2 = std::move(p1);
//    p2.x = 3;
//    std::cout << p2.x << p1.x << '\n';
    auto h = std::unordered_map<S, S, MyHash>();
    h.insert({1, 1});
    auto h2 = UnorderedMap<S, S, MyHash>();
    h2.insert({1, 1});

}
