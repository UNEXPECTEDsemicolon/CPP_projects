#include <iostream>
#include <ctime>
#include <cmath>
#include "../biginteger/biginteger.h"

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

template<size_t N, bool is_prime = IsPrime<N>::value>
class _AnyResidue {
protected:
    size_t val = 0;
public:
    explicit _AnyResidue(size_t x) : val((x % N + N) % N) {}

    _AnyResidue(const _AnyResidue<N> &orig) : val(orig.val) {}

    _AnyResidue() {}

    explicit _AnyResidue(int x) : val((x % int(N) + N) % N) {}
    
    _AnyResidue<N, is_prime> &operator= (const _AnyResidue<N> &orig) {
        val = orig.val;
        return *this;
    };

    explicit operator int() const {
        return val;
    }

    _AnyResidue<N, is_prime> &operator+=(const _AnyResidue<N> &right) {
        val = (val + right.val) % N;
        return *this;
    }

    _AnyResidue<N, is_prime> &operator-=(const _AnyResidue<N> &right) {
        val = (val + N - right.val) % N;
        return *this;
    }

    _AnyResidue<N, is_prime> &operator*=(const _AnyResidue<N> &right) {
        val = (val * right.val) % N;
        return *this;
    }

    _AnyResidue<N, is_prime> &operator++() {
        val = (val + 1) % N;
        return *this;
    }

    _AnyResidue<N, is_prime> operator++(int) {
        _AnyResidue<N, is_prime> res = val;
        ++*this;
        return res;
    }

    _AnyResidue<N, is_prime> &operator--() {
        val = (val + N - 1) % N;
        return *this;
    }

    _AnyResidue<N, is_prime> operator--(int) {
        _AnyResidue<N, is_prime> res = val;
        --*this;
        return res;
    }

    bool operator== (const _AnyResidue<N> &right) const {
        return val == right.val;
    }

    bool operator!= (const _AnyResidue<N> &right) const {
        return val != right.val;
    }

    _AnyResidue<N, is_prime> operator+(const _AnyResidue<N> &right) const {
        auto res = *this;
        return (res += right);
    }

    _AnyResidue<N, is_prime> operator-(const _AnyResidue<N> &right) const {
        auto res = *this;
        return (res -= right);
    }

    _AnyResidue<N, is_prime> operator-() const {
        return (_AnyResidue(0) - *this);
    }

    _AnyResidue<N, is_prime> operator*(const _AnyResidue<N> &right) const {
        auto res = *this;
        return (res *= right);
    }

    _AnyResidue<N, is_prime> pow(size_t n) const {
        if (n == 0) return _AnyResidue<N, is_prime>(1);
        if (n % 2 == 0) {
            auto res = pow(n / 2);
            return res * res;
        } else {
            auto res = *this;
            return pow(n - 1) * res;
        }
    }
    template<size_t _N>
    friend std::ostream &operator<< (std::ostream &out, const _AnyResidue<_N> &res);
};

template<size_t N>
std::ostream &operator<< (std::ostream &out, const _AnyResidue<N> &res) {
    out << res.val;
    return out;
}

template<size_t N, bool is_prime = IsPrime<N>::value>
class Residue: public _AnyResidue<N> {
protected:
    using _AnyResidue<N>::val;

public:
    Residue(size_t x): _AnyResidue<N>(x) {}

    Residue(const _AnyResidue<N> &orig): _AnyResidue<N>(orig) {}

    Residue(): _AnyResidue<N>() {}

    Residue(int x): _AnyResidue<N>(x) {} //explicit ?
};

template<size_t N>
class Residue<N, true>: public _AnyResidue<N> {
protected:
    using _AnyResidue<N>::val;

public:
    Residue(size_t x): _AnyResidue<N>(x) {}

    Residue(const _AnyResidue<N> &orig): _AnyResidue<N>(orig) {}

    Residue(): _AnyResidue<N>() {}

    Residue(int x): _AnyResidue<N>(x) {} //explicit ?

    Residue<N> &operator/= (const _AnyResidue<N, true> &right) {
        *this *= right.pow(N - 2);
        return *this;
    }
};

template<size_t N>
Residue<N, true> operator/ (const _AnyResidue<N, true> &left, const _AnyResidue<N, true> &right) {
    Residue<N, true> res = left;
    return (res /= right);
}

using std::vector;

template<size_t M, size_t N, typename Field = Rational>
class _AnyMatrix {
private:
    const Field _checking_for_prime_Residue = Field(1) / Field(1);
protected:
    vector<vector<Field>> body;

public:
    _AnyMatrix()
        : body(vector<vector<Field>>(M, vector<Field>(N, Field(0)))) {}

    template<typename T>
    _AnyMatrix(const vector<vector<T>> &source)
        : body(vector<vector<Field>>(M, vector<Field>(N))) {
        for (size_t i = 0; i < source.size(); ++i) {
            for (size_t j = 0; j < source[i].size(); ++j)
                body[i][j] = source[i][j];
        }
    }

    _AnyMatrix(const _AnyMatrix<M, N, Field> &orig): body(orig.body) {}

    _AnyMatrix<M, N, Field> &operator= (const _AnyMatrix<M, N, Field> &orig) {
        body = orig.body;
        return *this;
    }

    bool operator== (const _AnyMatrix<M, N, Field> &right) const {
        return body == right.body;
    }

    bool operator!= (const _AnyMatrix<M, N, Field> &right) const {
        return body != right.body;
    }

    _AnyMatrix<M, N, Field> &operator+= (const _AnyMatrix<M, N, Field> &right) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                body[i][j] += right.body[i][j];
        return *this;
    }

    _AnyMatrix<M, N, Field> operator+ (const _AnyMatrix<M, N, Field> &right) const {
        auto res = *this;
        return res += right;
    }

    _AnyMatrix<M, N, Field> &operator-= (const _AnyMatrix<M, N, Field> &right) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                body[i][j] -= right.body[i][j];
        return *this;
    }

    _AnyMatrix<M, N, Field> operator- (const _AnyMatrix<M, N, Field> &right) const {
        auto res = *this;
        return res -= right;
    }

    _AnyMatrix<M, N, Field> &operator*= (const Field &num) {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                body[i][j] *= num;
        return *this;
    }

    _AnyMatrix<M, N, Field> operator* (const Field &num) const {
        _AnyMatrix<M, N, Field> res(*this);
        return res *= num;
    }

    template<size_t K>
    _AnyMatrix<M, K, Field> operator* (const _AnyMatrix<N, K, Field> &right) const {
        _AnyMatrix<M, K, Field> res;
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < K; ++j) {
                for (size_t z = 0; z < N; ++z) {
                    res.body[i][j] += body[i][z] * right.body[z][j];
                }
            }
        }
        return res;
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

    _AnyMatrix<N, M, Field> transposed() const {
        _AnyMatrix<N, M, Field> res;
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                res.body[j][i] = body[i][j];
        return res;
    }

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

    size_t rank() {
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

    template<size_t _M, size_t _N, typename _Field>
    friend std::ostream &operator<< (std::ostream &out, const _AnyMatrix<_M, _N, _Field> &_AnyMatrix);
};

template<size_t M, size_t N, typename Field>
std::ostream &operator<< (std::ostream &out, const _AnyMatrix<M, N, Field> &_AnyMatrix) {
    for (size_t i = 0; i < _AnyMatrix.body.size(); ++i) {
        for (size_t j = 0; j < _AnyMatrix.body[i].size(); ++j)
            out << _AnyMatrix.body[i][j] << ' ';
        out << '\n';
    }
    return out;
}

template<size_t M, size_t N, typename Field = Rational>
class Matrix: public _AnyMatrix<M, N, Field> {
protected:
    using _AnyMatrix<M, N, Field>::body;
public:
    template<typename T>
    Matrix(const vector<vector<T>> &source)
            : _AnyMatrix<M, N, Field>(source) {}

    Matrix(const _AnyMatrix<M, N, Field> &orig)
            : _AnyMatrix<M, N, Field>(orig) {}
};

template<size_t N, typename Field>
class Matrix<N, N, Field>: public _AnyMatrix<N, N, Field> {
protected:
    using _AnyMatrix<N, N, Field>::body;

public:
    Matrix()
            : _AnyMatrix<N, N, Field>() {
        for (size_t i = 0; i < N; ++i)
            _AnyMatrix<N, N, Field>::body[i][i] = 1;
    }

    template<typename T>
    Matrix(const vector<vector<T>> &source)
            : _AnyMatrix<N, N, Field>(source) {}

    Matrix(const _AnyMatrix<N, N, Field> &orig)
            : _AnyMatrix<N, N, Field>(orig) {}

    Matrix<N, N, Field> &operator= (const _AnyMatrix<N, N, Field> &orig) {
        body = orig.body;
        return *this;
    }

    Matrix<N, N, Field> &operator*= (const _AnyMatrix<N, N, Field> &right) {
        return *this = *this * right;
    }

    Field det() const {
        auto copy = *this;
        Field res = 1;
        for (size_t begin = 0; begin < N; ++begin) {
            Field sign = 1;
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

    Field trace() {
        Field res = 0;
        for (size_t i = 0; i < N; ++i)
            res += body[i][i];
        return res;
    }

    Matrix<N, N, Field> inverted() {
        _AnyMatrix<N, N * 2, Field> ext;
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
};

template<size_t N, typename Field = Rational>
class SquareMatrix: public Matrix<N, N, Field>{
public:
    SquareMatrix()
        : Matrix<N, N, Field>() {}

    template<typename T>
    SquareMatrix(const vector<vector<T>> &source)
        : Matrix<N, N, Field>(source) {}

    SquareMatrix(const _AnyMatrix<N, N, Field> &orig)
        : Matrix<N, N, Field>(orig) {}
};


int main() {
//    auto sq = Matrix<4, 4>(vector<vector<int>>{{4, 4, 6, 7}, {8, 8, 12, 15}, {1, 6, 4, 5}, {7, 2, 5, 3}});
//    auto m1 = Matrix<3, 3>(vector<vector<int>>{{1, 45, 3}, {4, 82, 9}, {7, 3, 31}});
//    auto m2 = Matrix<3, 3, Residue<13>>(vector<vector<int>>{{1, 45, 3}, {4, 82, 9}, {7, 3, 31}});
//    auto E = SquareMatrix<1>();
////    auto n = Matrix<3, 4>(vector<vector<int>>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}});
////    auto sq = Matrix<3, 3>(vector<vector<int>>{{1 2 3}, {4, 5, 6}, {7, 8, 9}});
////    std::cout << sq;
////    std::cout << m * sq;
////    int two = 2;
////    std::cout << Rational(2) * m;
////    std::cout << m2._toTriangle();
//    std::cout << "----------\n";
////    std::cout << m2.transposed();
//    std::cout << m2.det() << '\n';
//    std::cout << sq << '\n';
//    std::cout << m1.det() << '\n';
//    std::cout << ((-3752 % -int(13ull) + int(13ull)) % int(13ull)) << '\n';
//    std::cout << m2.det() << '\n';

//    const size_t m = 10, n = 10;
////    std::cin >> m >> n;
//    vector<vector<int>> vec(m, vector<int>(n, 0));
//    for (size_t i = 0; i < m; ++i) {
//        for (size_t j = 0; j < n; ++j) {
//            std::cin >> vec[i][j];
//        }
//    }
//    auto matr = Matrix<m, n>(vec);
//    auto begin = clock();
//    std::cout << matr.det() << '\n';
//    std::cout << "time = " << (clock() - begin) / (double)CLOCKS_PER_SEC << '\n';
//    auto r = Residue<13>(2);
//    std::cout << Residue<13>(2) * Residue<13>(7) << '\n';
//    Residue<14> r(2);
//    const int &b = 2;
//    std::cout << (Residue<13>(-3752)) << '\n';
    std::cout << IsPrime<14>::value << '\n';
}