#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

class Complex {
public:
    double re, im;
    Complex(double re = 0, double im = 0): re(re), im(im) {}
    
    Complex(const Complex &orig): re(orig.re), im(orig.im) {}
    
    Complex &operator=(const Complex &orig) {
        re = orig.re;
        im = orig.im;
        return *this;
    }
    
    double abs() const {
        return sqrt(re * re + im * im);
    }
    
    Complex &operator+= (const Complex &right) {
        re += right.re;
        im += right.im;
        return *this;
    }
    
    Complex &operator-= (const Complex &right) {
        re -= right.re;
        im -= right.im;
        return *this;
    }
    
    Complex &operator*= (const Complex &right) {
        re = re * right.re - im * right.im;
        im = re * right.im + im * right.re;
        return *this;
    }
    
    Complex &operator/= (const Complex &right) {
        double denum = right.re * right.re + right.im * right.im;
        auto new_re = (re * right.re + im * right.im) / denum;
        auto new_im = (im * right.re - re * right.im) / denum;
        re = new_re;
        im = new_im;
        return *this;
    }
};

Complex operator+ (const Complex &left, const Complex &right) {
    return Complex(left) += right;
}
Complex operator- (const Complex &left, const Complex &right) {
    return Complex(left) -= right;
}
Complex operator* (const Complex &left, const Complex &right) {
    return Complex(left) *= right;
}
Complex operator/ (const Complex &left, const Complex &right) {
    return Complex(left) /= right;
}

bool operator== (const Complex &left, const Complex &right) {
    return std::abs(left.re - right.re) < 1e-10 && std::abs(left.im - right.im) < 1e-10;
}

bool operator!= (const Complex &left, const Complex &right) {
    return !(left == right);
}

std::ostream & operator << (std::ostream &out, const Complex &t) {
    out << t.re << ' ' << t.im;
    return out;
}

int main() {
    size_t n;
    std::ifstream in("matrix.in");
    in >> n;
    auto a = std::vector<std::vector<Complex>>(n, std::vector<Complex>(n * 2, 0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            in >> a[i][j].re >> a[i][j].im;
        a[i][n + i] = 1;
    }
    in.close();

    for (size_t begin = 0; begin < n; ++begin) {
        for (size_t i = begin; i < n; ++i) {
            if (a[i][begin] != 0) {
                std::swap(a[i], a[begin]);
                break;
            }
        }
        if (a[begin][begin] == 0.0)
            return 1;
        auto temp = a[begin][begin];
        for (size_t i = begin; i < n * 2; ++i)
            a[begin][i] /= temp;
        for (size_t i = begin + 1; i < n; ++i) {
            auto temp = a[i][begin];
            for (size_t j = begin; j < n * 2; ++j) {
                a[i][j] -= temp * a[begin][j];
            }
        }
    }
    for (size_t i = 1; i < n; ++i) {
        for (size_t j = 0; j < i; ++j) {
            auto temp = a[j][i];
            for (size_t z = 0; z < 2 * n; ++z) {
                a[j][z] -= a[i][z] * temp;
            }
        }
    }
    std::ofstream out("matrix.out");
    out.setf(std::ios::fixed);
    out.precision(15);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = n; j < n * 2; ++j)
            out << a[i][j] << ' ';
        out << '\n';
    }
    out.close();
}