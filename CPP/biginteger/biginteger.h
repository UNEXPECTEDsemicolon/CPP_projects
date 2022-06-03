#include <iostream>
#include <vector>
#include <string>

class Rational;

class BigInteger {
private:
    static const size_t RADIX = 1ull << (sizeof(size_t) / 2 * 8);
    size_t n = 0;
    std::vector<size_t> body = {};
    int sign = 1;
    size_t div_and_get_digit(size_t x = 10) {
        size_t mod = 0, old_mod = 0;
        for (size_t i = n - 1; i + 1 >= 1; i--) {
            mod = (body[i] + old_mod * RADIX) % x;
            body[i] = (body[i] + old_mod * RADIX) / x;
            old_mod = mod;
        }
        clear_begin();
        return mod;
    }
    bool is_zero() const {
        return (n == 1 && body[0] == 0);
    }
    static BigInteger abs(const BigInteger &BigInt) {
        auto result = BigInt;
        result.sign = 1;
        return result;
    }
    BigInteger &abs() {
        sign = 1;
        return *this;
    }
    BigInteger &clear_begin() {
        for (; n > 1 && !(body[n-1] || body[n-2]); --n) body.pop_back();
        return *this;
    }
    int cmp_abs(const BigInteger &right) const {
        if (n < right.n) return -1;
        else if (n > right.n) return 1;
        for (size_t i = n - 1; i + 1 >= 1; i--) {
            if (body[i] < right.body[i]) return -1;
            else if (body[i] > right.body[i]) return 1;
        }
        return 0;
    }
    int cmp(const BigInteger &right) const {
        if (sign < 0 && right.sign > 0) return -1;
        if (sign > 0 && right.sign < 0) return 1;
        return cmp_abs(right) * sign;
    }
    BigInteger &add(const BigInteger& left, const BigInteger& right) {
        body.resize(n = (right.n > left.n ? right.n : left.n) + 1, 0);
        size_t carry = 0;
        for (size_t i = 0; i < right.n || (carry && i < n); i++) {
            body[i] = left.body[i] + (i < right.n ? right.body[i] : 0) + carry;
            carry = body[i] / RADIX;
            body[i] %= RADIX;
        }
        return clear_begin();
    }
    BigInteger &sub(const BigInteger& left, const BigInteger& right) {
        bool flag = (left.cmp_abs(right) < 0);
        const BigInteger &maxpart = flag ? right : left;
        const BigInteger &minpart = flag ? left : right;
        body.resize(n = maxpart.n);
        if (maxpart.sign * minpart.sign > 0) sign = maxpart.sign * (&maxpart == &left ? 1 : -1);
        else sign = (&maxpart == &left ? 1 : -1);
        size_t carry = 0;
        for (size_t i = 0; i < minpart.n || (carry && i < maxpart.n); ++i) {
            size_t temp_sum = carry + (i < minpart.n ? minpart.body[i] : 0);
            body[i] = maxpart.body[i] + RADIX * (carry = (maxpart.body[i] < temp_sum)) - temp_sum;
        }
        return clear_begin();
    }
    BigInteger &mul(const BigInteger& left, const BigInteger& right) {
        sign = left.sign * right.sign;
        body.assign(n = left.n + right.n, 0);
        size_t digit, carry, final_carry, old_final_carry;
        for (size_t j = 0; j < right.n; j++) {
            carry = final_carry = old_final_carry = 0;
            for (size_t i = 0; i < left.n; i++) {
                digit = left.body[i] * right.body[j] + carry;
                carry = digit / RADIX;
                final_carry = (body[i + j] + digit % RADIX + final_carry) / RADIX;
                body[i + j] = (body[i + j] + digit % RADIX + old_final_carry) % RADIX;
                old_final_carry = final_carry;
            }
        }
        return clear_begin();
    }
    size_t easy_div(const BigInteger &right, BigInteger &sum_for_check) {
        if (cmp_abs(right) < 0) return 0;
        size_t l = 1, r = RADIX - 1, m;
        (sum_for_check = right).abs();
        for (; r - l > 1; (sum_for_check = right).abs()) {
            sum_for_check *= (m = (l + r) / 2);
            if (cmp_abs(sum_for_check) > 0) l = m;
            else if (cmp_abs(sum_for_check) < 0) r = m;
            else l = r = m;
        }
        size_t res;
        if (cmp_abs(sum_for_check *= r) >= 0) res = r;
        else (sum_for_check = right).abs() *= (res = l);
        *this -= sum_for_check;
        return res;
    }
    BigInteger &div_and_mod(const BigInteger &left, const BigInteger &right, BigInteger &mod) {
        sign = left.sign * right.sign;
        if (left.cmp_abs(right) < 0) {
            mod = left;
            return *this = 0;
        }
        BigInteger copy_right(right);
        for (size_t i = left.n - 1; i + 1 >= 1; i--) {
            (mod *= RADIX).body[0] += left.body[i];
            body[i] = (mod *= 1).clear_begin().easy_div(right, copy_right);
        }
        if (sign < 0 && !mod.is_zero()) {
            body[0] += 1;
            *this *= 1;
        }
        return clear_begin();
    }
public:
    BigInteger(const long long &val = 0): n((bool)val + 1), sign((val >= 0) ? 1 : -1) {
        body.resize(n);
        body[0] = (val >= 0) ? val : -val;
    }
    BigInteger(const std::string &s) {
        body.resize(1);
        size_t i = 0;
        if (s[0] == '-') {
            sign = -1;
            ++i;
        }
        for (; i < s.length(); ++i) {
            *this *= 10;
            body[0] += (size_t)(s[i] - '0');
        }
        *this *= 1;
        clear_begin();

    }
    BigInteger &operator= (const BigInteger &orig) {
        n = orig.n;
        body = orig.body;
        sign = orig.sign;
        return *this;
    }
    BigInteger(const BigInteger &orig) {
        *this = orig;
    }
    ~BigInteger() {
    }
    std::string toString() const {
        BigInteger num_for_div(*this);
        num_for_div.abs();
        if (is_zero()) return std::string("0");
        std::string str = "";
        for (size_t i = 0; !num_for_div.is_zero(); ++i)
            str.push_back('0' + (char)(num_for_div.div_and_get_digit()));
        if (sign < 0) str.push_back('-');
        for (size_t i = 0; i < str.length() / 2; i++)
            std::swap(str[i], str[str.length() - i - 1]);
        return str;
    }
    void show() const {
        if (sign < 0) std::cout << '-';
        for (size_t i = 0; i < n; ++i)
            std::cout << body[i] << ' ';
        std::cout << '\n';
    }
    friend bool operator< (const BigInteger &left, const BigInteger &right);
    friend bool operator> (const BigInteger &left, const BigInteger &right);
    friend bool operator<= (const BigInteger &left, const BigInteger &right);
    friend bool operator>= (const BigInteger &left, const BigInteger &right);
    friend bool operator== (const BigInteger &left, const BigInteger &right);
    friend bool operator!= (const BigInteger &left, const BigInteger &right);
    explicit operator bool() const {
        return !is_zero();
    }
    BigInteger operator- () {
        BigInteger result(*this);
        result.sign *= -1;
        return result;
    }
    BigInteger &operator+= (const BigInteger &right) {
        if (sign * right.sign < 0) return (right.sign < 0) ? sub(*this, right) : sub(right, *this);
        return add(*this, right);
    }
    BigInteger &operator++ () {
        return *this += 1;
    }
    BigInteger operator++ (int) {
        auto result = *this;
        ++(*this);
        return result;
    }
    BigInteger &operator-= (const BigInteger &right) {
        if (sign * right.sign < 0) return add(*this, right);
        return sub(*this, right);
    }
    BigInteger &operator-- () {
        return *this -= 1;
    }
    BigInteger operator-- (int) {
        auto result = *this;
        --(*this);
        return result;
    }
    BigInteger &operator*= (const size_t x) {
        body.push_back(0); ++n;
        for (size_t i = 0, carry = 0; i < n; ++i) {
            body[i] = body[i] * x + carry;
            if (body[i] >= RADIX) {
                carry = body[i] / RADIX;
                body[i] %= RADIX;
            } else carry = 0;
        }
        clear_begin();
        return *this;
    }
    BigInteger &operator*= (const BigInteger &right) {
        auto left = *this;
        if (this != &right) return mul(left, right);
        return mul(left, left);
    }
    BigInteger &operator/= (const BigInteger &right) {
        BigInteger mod = 0;
        return div_and_mod(*this, right, mod);
    }
    BigInteger &operator%= (const BigInteger &right) {
        BigInteger result = 0;
        div_and_mod(*this, right, result);
        return *this = result;
    }
    friend Rational;
};

bool operator< (const BigInteger &left, const BigInteger &right) {
    return (left.cmp(right) < 0);
}

bool operator> (const BigInteger &left, const BigInteger &right) {
    return (left.cmp(right) > 0);
}

bool operator<= (const BigInteger &left, const BigInteger &right) {
    return (left.cmp(right) <= 0);
}

bool operator>= (const BigInteger &left, const BigInteger &right) {
    return (left.cmp(right) >= 0);
}

bool operator== (const BigInteger &left, const BigInteger &right) {
    return (left.cmp(right) == 0);
}

bool operator!= (const BigInteger &left, const BigInteger &right) {
    return (left.cmp(right) != 0);
}

BigInteger operator+ (const BigInteger &left, const BigInteger &right) {
    BigInteger result(left);
    return result += right;
}

BigInteger operator- (const BigInteger &left, const BigInteger &right) {
    BigInteger result(left);
    return result -= right;
}

BigInteger operator* (const BigInteger &left, const BigInteger &right) {
    BigInteger result(left);
    return result *= right;
}

BigInteger operator/ (const BigInteger &left, const BigInteger &right) {
    BigInteger result(left);
    return result /= right;
}

BigInteger operator% (const BigInteger &left, const BigInteger &right) {
    BigInteger result(left);
    return result %= right;
}

std::ostream &operator<< (std::ostream &out, const BigInteger &BigInt) {
    out << BigInt.toString();
    return out;
}

std::istream &operator>> (std::istream &in, BigInteger &BigInt) {
    std::string str;
    in >> str;
    BigInt = BigInteger(str);
    return in;
}


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
    Rational(const int num, const int denom = 1): num(num), denom(denom) {
        normal();
    }
    Rational(const Rational &orig) = default;
    Rational &operator=(const Rational &orig) = default;
    ~Rational() {};
    friend bool operator< (const Rational &left, const Rational &right);
    friend bool operator> (const Rational &left, const Rational &right);
    friend bool operator== (const Rational &left, const Rational &right);
    friend bool operator!= (const Rational &left, const Rational &right);
    friend bool operator<= (const Rational &left, const Rational &right);
    friend bool operator>= (const Rational &left, const Rational &right);
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
    Rational &operator++ () {
        return *this += 1;
    }
    Rational operator++ (int) {
        auto result = *this;
        ++(*this);
        return result;
    }
    Rational &operator-= (Rational const &right) {
        num = num * right.denom - right.num * denom;
        denom *= right.denom;
        normal();
        return *this;
    }
    Rational &operator-- () {
        return *this -= 1;
    }
    Rational operator-- (int) {
        auto result = *this;
        --(*this);
        return result;
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

bool operator< (const Rational &left, const Rational &right) {
    return (left.num * right.denom < right.num * left.denom);
}

bool operator> (const Rational &left, const Rational &right) {
    return right < left;
}

bool operator== (const Rational &left, const Rational &right) {
    return left.num == right.num;
}

bool operator!= (const Rational &left, const Rational &right) {
    return !(left == right);
}

bool operator<= (const Rational &left, const Rational &right) {
    return left == right || left < right;
}

bool operator>= (const Rational &left, const Rational &right) {
    return right <= left;
}

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

