#include "smart_pointers.h"

struct S {
    int x;
    S(int x): x(x) {}

    S(const S& s): x(s.x) {}
};

struct P: S {
    int y;
    P(const S& s): S::S(s), y(s.x) {}

    P(int y): S::S(y), y(y) {}
};

int main() {
//    P p(5);
//    std::cout << p.x << '\n';
    auto a = SharedPtr<P>(new P(5));
    auto s = SharedPtr<S>(a);
//    typeid(is_base<S, P>);
//    std::cout << s->x << '\n';
    WeakPtr<P> ws = a;
//    std::cout << std::is_base_of<int, int>::value << '\n';
    auto abc = makeShared<int>(8);
    auto abc2 = abc;
    std::cout << abc2.use_count() << '\n';
}