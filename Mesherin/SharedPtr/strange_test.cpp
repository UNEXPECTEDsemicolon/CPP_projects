//#include <iostream>
//#include <type_traits>
//
//struct S {
//    int x;
//};
//
//struct P {
//    int y;
//
//    P(const S& s): y(s.x) {}
//
//    P(int y): y(y) {}
//};
//
//int main() {
////    auto s = new S();
////    std::cout << "\n";
////    std::cout << std::is_base_of<S, S>::value << '\n';
////    S* s;
//    P p(1);
//    S* s = &p;
//}