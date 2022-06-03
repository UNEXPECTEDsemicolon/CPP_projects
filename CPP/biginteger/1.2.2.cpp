#include "1.2.1.cpp"

class Rational;
Rational operator+ (const Rational &left, const Rational &right);

class Rational {
private:
    BigInteger num;
    BigInteger denom;
    static BigInteger gcd(BigInteger left, BigInteger right) {
        left.abs(); right.abs();
        while (right) {
            std::swap(left, right);
            right %= left;
        }
        return left;
    }
    void normal() {
        num.sign *= denom.sign;
        denom.abs();
        BigInteger GCD = gcd(num, denom);
        num /= GCD;
        denom /= GCD;
    }
    void get_and_write_digit(std::string &str, BigInteger &copy_num, BigInteger &mod) const {
        copy_num.div_and_mod(copy_num, denom, mod);
        str += copy_num.toString();
        (copy_num = mod) *= 10;
        mod = 0;
    }
public:
    Rational(const BigInteger &num = 0, const BigInteger &denom = 1): num(num), denom(denom) {
        normal();
    }
    Rational(const int num = 0, const int denom = 1): num(num), denom(denom) {
        normal();
    }
    Rational(const Rational &orig) = default;
    Rational &operator=(const Rational &orig) = default;
    ~Rational() {};
    bool operator< (const Rational &right) const {
        return (num * right.denom < right.num * denom);
    }
    bool operator> (const Rational &right) const {
        return right < *this;
    }
    bool operator== (const Rational &right) const {
        return num == right.num;
    }
    bool operator!= (const Rational &right) const {
        return !(*this == right);
    }
    bool operator<= (const Rational &right) const {
        return *this == right || *this < right;
    }
    bool operator>= (const Rational &right) const {
        return right <= *this;
    }
    Rational operator- () {
        Rational result(*this);
        result.num.sign *= -1;
        return result;
    }
    Rational &operator+= (const Rational &right) {
        num = num * right.denom + right.num * denom;
        denom *= right.denom;
        normal();
        return *this;
    }
    Rational &operator-= (Rational const &right) {
        num = num * right.denom - right.num * denom;
        denom *= right.denom;
        normal();
        return *this;
    }
    Rational &operator*= (Rational const &right) {
        num *= right.num;
        denom *= right.denom;
        normal();
        return *this;
    }
    Rational &operator/= (Rational const &right) {
        num *= right.denom;
        denom *= right.num;
        normal();
        return *this;
    }
    std::string toString() const {
        std::string str(num.toString());
        if (denom != 1) (str += "/") += denom.toString();
        return str;
    }
    std::string asDecimal(size_t precision = 0, bool round = true) const {
        std::string str;
        if (num.sign < 0) str += "-";
        BigInteger copy_num(num), mod;
        get_and_write_digit(str, copy_num.abs(), mod);
        if (precision > 0) str += ".";
        for (size_t i = 0; i < precision + 1; ++i)
            get_and_write_digit(str, copy_num, mod);
        if (round && *(str.end() - 1) >= '5') {
            Rational eps(('9' + 1 - *(str.end() - 1)) * num.sign, "1" + std::string(precision + 1, '0'));
            return (*this + eps).asDecimal(precision - 1, false);
        }
        str.pop_back();
        return str != "-0" ? str : std::string("0");
    }
    explicit operator double() const {
        return atof(asDecimal(33).c_str());
    }
};

Rational operator+ (const Rational &left, const Rational &right) {
    Rational result(left);
    return result += right;
}

Rational operator- (const Rational &left, const Rational &right) {
    Rational result(left);
    return result -= right;
}

Rational operator* (const Rational &left, const Rational &right) {
    Rational result(left);
    return result *= right;
}
Rational operator/ (const Rational &left, const Rational &right) {
    Rational result(left);
    return result /= right;
}

std::ostream &operator<< (std::ostream &out, const Rational &frac) {
    out << frac.toString();
    return out;
}

int main() {
    auto a = Rational(-7864516, 453151);
    auto b = Rational(-2, 3);
    std::cout << a.asDecimal(100) << '\n';
    std::cout << a << '\n';
}