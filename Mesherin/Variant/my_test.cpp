#include "variant.h"
#include <variant>
#include <vector>

int main() {
//    auto v = Variant<double>(5);
//    auto v2 = std::variant<double, char>(short(5));
//    std::cout << std::get<0>(v2) << '\n';
//    std::cout << my::get<0>(v);
//    v = 1.2;
//    std::cout << my::get<1>(v);
    {
//        const int x = 5;
//        Variant<int, float> v(5.4);
//        Variant<int, double> v(5.4f);
//        std::cout << VariantAlternative<int, int, double>::constructible_check<float, int, double>::value << '\n';
//        v = Variant<int, double>(1.3);
//        std::cout << get<1>(v) << '\n';
//        v = Variant<int, double>(8);
//        std::cout << get<int>(v) << '\n';
    }

    {
//        Variant<const int, /*int,*/ std::string, const std::string, double> v = 1;
        Variant<const int, /*int,*/ std::string,/* const std::string,*/ double> v = 1;
//        std::cout << std::is_same<decltype(get<0>(v)), const int&>::value << '\n';
//        Variant<int, std::string, double> v("abc");
        v = "abc";
    }
    {
//        Variant<std::string, char, std::vector<int>> v = "abcdefgh";
//        v = 'a';
    }
}