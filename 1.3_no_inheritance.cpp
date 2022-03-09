#include <iostream>
#include <cmath>
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
    friend std::istream &operator>> (std::istream &in, const Rational &frac);
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

std::istream &operator>> (std::istream &in, const Rational &frac) {
    in >> frac.num;
    return in;
}



template<size_t N, size_t K>
struct IsPrimeHelper {
    static const bool value = (K == N || N % K != 0) && IsPrimeHelper<N, K - 1>::value;
};

template<size_t N>
struct IsPrimeHelper<N, 1> {
    static const bool value = N != 1;
};

template<size_t N>
struct IsPrime {
    static const bool value = IsPrimeHelper<N, size_t(ceil(sqrt(N)))>::value;
};

template<size_t N>
class Residue {
private:
    size_t val = 0;
public:
    explicit Residue(size_t x) : val((x % N + N) % N) {}

    Residue(const Residue<N> &orig) : val(orig.val) {}

    Residue() {}

    explicit Residue(int x) : val((x % (long long)(N) + N) % N) {}

    Residue<N> &operator= (const Residue<N> &orig) {
        val = orig.val;
        return *this;
    };

    explicit operator int() const {
        return val;
    }

    Residue<N> &operator+=(const Residue<N> &right) {
        val = (val + right.val) % N;
        return *this;
    }

    Residue<N> &operator-=(const Residue<N> &right) {
        val = (val + N - right.val) % N;
        return *this;
    }

    Residue<N> &operator*=(const Residue<N> &right) {
        val = (val * right.val) % N;
        return *this;
    }

    Residue<N> &operator++() {
        val = (val + 1) % N;
        return *this;
    }

    Residue<N> operator++(int) {
        Residue<N> res = val;
        ++*this;
        return res;
    }

    Residue<N> &operator--() {
        val = (val + N - 1) % N;
        return *this;
    }

    Residue<N> operator--(int) {
        Residue<N> res = val;
        --*this;
        return res;
    }

    bool operator== (const Residue<N> &right) const {
        return val == right.val;
    }

    bool operator!= (const Residue<N> &right) const {
        return val != right.val;
    }

    Residue<N> operator+(const Residue<N> &right) const {
        auto res = *this;
        return (res += right);
    }

    Residue<N> operator-(const Residue<N> &right) const {
        auto res = *this;
        return (res -= right);
    }

    Residue<N> operator-() const {
        return (Residue(0) - *this);
    }

    Residue<N> operator*(const Residue<N> &right) const {
        auto res = *this;
        return (res *= right);
    }

    Residue<N> pow(size_t n) const {
        if (n == 0) return Residue<N>(1);
        if (n % 2 == 0) {
            auto res = pow(n / 2);
            return res * res;
        } else {
            auto res = *this;
            return pow(n - 1) * res;
        }
    }

    Residue<N> &operator/= (const Residue<N> &right) {
        static_assert(IsPrime<N>::value, "N must be prime for for dividing!");
        *this *= right.pow(N - 2);
        return *this;
    }

    Residue<N> operator/ (const Residue<N> &right) {
        auto res = *this;
        return (res /= right);
    }

    template<size_t _N>
    friend std::ostream &operator<< (std::ostream &out, const Residue<_N> &res);
};

template<size_t N>
std::ostream &operator<< (std::ostream &out, const Residue<N> &res) {
    out << res.val;
    return out;
}

using std::vector;

template<size_t M, size_t N, typename Field = Rational>
class Matrix {
protected:
    vector<vector<Field>> body;

    Matrix(int)
            : body(vector<vector<Field>>(M, vector<Field>(N, Field(0)))) {}
public:
    Matrix()
            : body(vector<vector<Field>>(M, vector<Field>(N, Field(0)))) {
        if (N == M)
            for (size_t i = 0; i < N; ++i)
                body[i][i] = Field(1);
    }

    template<typename T>
    Matrix(const vector<vector<T>> &source)
            : body(vector<vector<Field>>(M, vector<Field>(N))) {
        for (size_t i = 0; i < source.size(); ++i) {
            for (size_t j = 0; j < source[i].size(); ++j)
                body[i][j] = Field(source[i][j]);
        }
    }

    template<typename T>
    Matrix(const std::initializer_list<std::initializer_list<T>> &source)
            : body(vector<vector<Field>>(M, vector<Field>(N))) {
        size_t i = 0;
        for (auto it1 = source.begin(); it1 != source.end(); ++it1, ++i) {
            size_t j = 0;
            for (auto it2 = it1->begin(); it2 != it1->end(); ++it2, ++j)
                body[i][j] = Field(*it2);
        }
    }

    Matrix(const Matrix<M, N, Field> &orig): body(orig.body) {}

    Matrix<M, N, Field> &operator= (const Matrix<M, N, Field> &orig) {
        body = orig.body;
        return *this;
    }

    bool operator== (const Matrix<M, N, Field> &right) const {
        return body == right.body;
    }

    bool operator!= (const Matrix<M, N, Field> &right) const {
        return body != right.body;
    }

    Matrix<M, N, Field> &operator+= (const Matrix<M, N, Field> &right) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                body[i][j] += right.body[i][j];
        return *this;
    }

    Matrix<M, N, Field> &operator-= (const Matrix<M, N, Field> &right) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                body[i][j] -= right.body[i][j];
        return *this;
    }

    Matrix<M, N, Field> &operator*= (const Field &num) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                body[i][j] *= num;
        return *this;
    }

    template<size_t _M, size_t _N, size_t _K, typename _Field>
    friend Matrix<_M, _K, _Field> operator* (const Matrix<_M, _N, _Field> &left, const Matrix<_N, _K, _Field> &right);

    Matrix<N, N, Field> &operator*= (const Matrix<N, N, Field> &right) {
        static_assert(N == M, "Only square matrices have operator *=");
        return *this = *this * right;
    }

    vector<Field> &operator[] (size_t ind) {
        return body[ind];
    }

    const vector<Field> &operator[] (size_t ind) const {
        return body[ind];
    }

    vector<Field> getRow(size_t ind) const {
        return body[ind];
    }

    vector<Field> getColumn(size_t ind) const {
        vector<Field> res(M);
        for (size_t i = 0; i < M; ++i) {
            res[i] = body[i][ind];
        }
        return res;
    }

    Matrix<N, M, Field> transposed() const {
        Matrix<N, M, Field> res;
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                res.body[j][i] = body[i][j];
        return res;
    }

protected:
    void _toTriangle() {
        size_t K = std::min(M, N);
        for (size_t begin = 0; begin < K; ++begin) {
            for (size_t i = begin; i < M; ++i) {
                if (body[i][begin] != Field(0)) {
                    std::swap(body[i], body[begin]);
                    break;
                }
            }
            if (body[begin][begin] == Field(0)) continue;
            auto temp = body[begin][begin];
            for (size_t i = begin; i < N; ++i) {
                body[begin][i] /= temp;
            }
            for (size_t i = begin + 1; i < M; ++i) {
                auto temp = body[i][begin];
                for (size_t j = begin; j < N; ++j) {
                    body[i][j] -= temp * body[begin][j];
                }
            }
        }
    }

public:
    size_t rank() const {
        auto copy = *this;
        size_t K = std::min(M, N);
        copy._toTriangle();
        size_t res1 = 0;
        for (size_t j = 0; j < K; ++j) {
            for (size_t i = 0; i < K; ++i) {
                if (copy.body[i][j] != Field(0)) {
                    ++res1;
                    break;
                }
            }
        }
        size_t res2 = 0;
        for (size_t i = 0; i < K; ++i) {
            for (size_t j = 0; j < K; ++j) {
                if (copy.body[i][j] != Field(0)) {
                    ++res2;
                    break;
                }
            }
        }
        return std::min(res1, res2);
    }

    Field det() const {
        static_assert(N == M, "Only square matrices have det");
        auto copy = *this;
        auto res = Field(1);
        for (size_t begin = 0; begin < N; ++begin) {
            auto sign = Field(1);
            if (copy.body[begin][begin] == Field(0)) {
                for (size_t i = begin; i < N; ++i) {
                    if (copy.body[i][begin] != Field(0)) {
                        std::swap(copy.body[i], copy.body[begin]);
                        sign *= Field(-1);
                        break;
                    }
                }
                if (sign == Field(1)) return Field(0);
            }
            for (size_t i = begin + 1; i < N; ++i) {
                auto a12 = copy.body[begin][i];
                for (size_t j = begin; j < N; ++j)
                    copy.body[j][i] += copy.body[j][begin] * (-a12 / copy.body[begin][begin]);
            }
            res *= copy.body[begin][begin] * sign;
        }
        return res;
    }

    Field trace() const {
        static_assert(N == M, "Only square matrices have trace");
        auto res = Field(0);
        for (size_t i = 0; i < N; ++i)
            res += body[i][i];
        return res;
    }

    Matrix<N, N, Field> inverted() const {
        static_assert(N == M, "Only square matrices have can be inverted");
        Matrix<N, N * 2, Field> ext;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                ext.body[i][j] = body[i][j];
            }
        }
        for (size_t i = 0; i < N; ++i) {
            ext.body[i][N + i] = Field(1);
        }
        ext._toTriangle();
        for (size_t i = 1; i < N; ++i) {
            for (size_t j = 0; j < i; ++j) {
                auto temp = ext.body[j][i];
                for (size_t z = 0; z < 2 * N; ++z) {
                    ext.body[j][z] -= ext.body[i][z] * temp;
                }
            }
        }
        auto res = Matrix<N, N, Field>();
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                res.body[i][j] = ext[i][N + j];
            }
        }
        return res;
    }

    void invert() {
        *this = inverted();
    }

    template<size_t _M, size_t _N, typename _Field>
    friend std::ostream &operator<< (std::ostream &out, const Matrix<_M, _N, _Field> &Matrix);
    friend Matrix<N, M, Field>;
    friend Matrix<M, N / 2, Field>;
};

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator+ (const Matrix<M, N, Field> &left, const Matrix<M, N, Field> &right) {
    auto res = left;
    return res += right;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator- (const Matrix<M, N, Field> &left, const Matrix<M, N, Field> &right) {
    auto res = left;
    return res -= right;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator* (const Matrix<M, N, Field> &left, const Field &num) {
    auto res = left;
    return res *= num;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator* (const Field &num, const Matrix<M, N, Field> &right) {
    return right * num;
}

template<size_t M, size_t N, size_t K, typename Field>
Matrix<M, K, Field> operator* (const Matrix<M, N, Field> &left, const Matrix<N, K, Field> &right) {
    Matrix<M, K, Field> res(0);
    auto new_right = right;
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < K; ++j) {
            for (size_t z = 0; z < N; ++z) {
                res.body[i][j] += left.body[i][z] * new_right.body[z][j];
            }
        }
    }
    return res;
}

template<size_t M, size_t N, typename Field>
std::ostream &operator<< (std::ostream &out, const Matrix<M, N, Field> &Matrix) {
    for (size_t i = 0; i < Matrix.body.size(); ++i) {
        for (size_t j = 0; j < Matrix.body[i].size(); ++j)
            out << Matrix.body[i][j] << ' ';
        out << '\n';
    }
    return out;
}

template<size_t N, typename Field = Rational>
class SquareMatrix: public Matrix<N, N, Field>{
public:
    SquareMatrix()
            : Matrix<N, N, Field>() {}

    template<typename T>
    SquareMatrix(const vector<vector<T>> &source)
            : Matrix<N, N, Field>(source) {}

    template<typename T>
    SquareMatrix(const std::initializer_list<std::initializer_list<T>> &source)
            : Matrix<N, N, Field>(source) {}

    SquareMatrix(const Matrix<N, N, Field> &orig)
            : Matrix<N, N, Field>(orig) {}
};