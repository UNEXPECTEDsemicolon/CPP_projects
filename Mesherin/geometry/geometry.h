#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

const double EPS = 1e-5;

class Line;

class Point {
public:
    double x = 0, y = 0;

    Point(double x, double y)
        : x(x), y(y) {};

    Point(const Point &orig)
        : x(orig.x), y(orig.y) {}

    Point &operator=(const Point &orig) {
        x = orig.x;
        y = orig.y;
        return *this;
    }

    bool operator==(const Point &right) const {
        return std::abs(x - right.x) < EPS && std::abs(y - right.y) < EPS;
    }

    bool operator!=(const Point &right) const {
        return !(*this == right);
    }

    Point &operator+=(const Point &right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    Point operator+(const Point &right) const {
        return Point(*this) += right;
    }

    Point &operator-=(const Point &right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    Point operator-(const Point &right) const {
        return Point(*this) -= right;
    }

    Point operator-() const {
        return Point(-x, -y);
    }

    Point& operator*=(double num) {
        x *= num;
        y *= num;
        return *this;
    }

    Point& operator/=(double num) {
        x /= num;
        y /= num;
        return *this;
    }

    double abs() const {
        return sqrt(x*x + y*y);
    }

    Point& rotate(const Point& center, double angle) {
        angle *= M_PI / 180;
        *this -= center;
        double new_x = x * cos(angle) - y * sin(angle);
        double new_y = x * sin(angle) + y * cos(angle);
        x = new_x;
        y = new_y;
        return *this += center;
    }

    void reflect(const Point& center);
    void reflect(const Line& axis);
    void scale(const Point& center, double coeff);
};

Point operator*(const Point& point, double num) {
    return Point(point) *= num;
}

Point operator*(double num, const Point& point) {
    return point * num;
}

Point operator/(const Point& point, double num) {
    return Point(point) /= num;
}

std::ostream &operator<<(std::ostream& out, const Point& point) {
    out << "(" << point.x << ", " << point.y << ")";
    return out;
}

class Vector: public Point {
public:
    using Point::x;
    using Point::y;

    Vector(double x, double y, bool norm = true)
        : Point::Point(x, y) {
        if (norm && (std::abs(x) > EPS || std::abs(y) > EPS)) {
            double new_x = x / sqrt(x * x + y * y);
            double new_y = y / sqrt(x * x + y * y);
            this->x = new_x;
            this->y = new_y;
        }
    }

    Vector(const Point &point, bool norm = true): Vector(point.x, point.y, norm) {}

    Vector(const Vector &orig): Point::Point(orig.x, orig.y) {}
};

double operator*(const Vector& left, const Vector& right) {
    return left.x * right.x + left.y * right.y;
}

double getAbsVecProd(const Vector& left, const Vector& right) {
    return left.x * right.y - right.x * left.y;
}

double getAngle(const Vector& left, const Vector& right) {
    return acos(left * right / (left.abs() * right.abs()));
}

class Line {
public:
    Point r0;
    Vector a;
    double k, b;
    Line(const Point &first, const Point &second)
        : r0(first), a(second - first) {}

    Line(double k, double b)
        : r0(1, k + b), a(1, k) {}

    Line(const Point &point, double k)
        : r0(point), a(k, 1) {}

    bool operator==(const Line &right) const {
        return a == Vector(r0 - right.r0) || a == -Vector(r0 - right.r0);
    }

    bool operator!=(const Line &right) const {
        return !(*this == right);
    }
};

void Point::reflect(const Point& center) {
    *this += 2 * (center - *this);
}

void Point::reflect(const Line& axis) {
    double a, b, c;
    a = axis.a.y;
    b = -axis.a.x;
    c = axis.a.x * axis.r0.y - axis.a.y * axis.r0.x;
    auto norm = Vector(axis.a.y, -axis.a.x);
    double d = std::abs(Vector(*this - axis.r0, false) * norm);
    double sign = a * x + b * y + c;
    if (std::abs(sign) > EPS)
        sign /= std::abs(sign);
    else
        sign = 0.0;
    *this -= sign * 2 * d * norm;
}

void Point::scale(const Point& center, double coeff) {
    *this += (coeff - 1) * Vector(*this - center, false);
}

class Shape {
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator==(const Shape& another) const = 0;
    bool operator!=(const Shape& another) const {
        return !(*this == another);
    }
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(const Point& point) const = 0;
    virtual void rotate(const Point& center, double angle) = 0;
    virtual void reflect(const Point& center) = 0;
    virtual void reflect(const Line& axis) = 0;
    virtual void scale(const Point& center, double coeff) = 0;

    virtual ~Shape() = default;
};

std::ostream &operator<<(std::ostream& out, const std::vector<Point>& pol) {
    out << "polygon(";
    for (size_t i = 0; i < pol.size(); ++i)
        out << pol[i] << ", ";
    out << ")";
    return out;
}

class Polygon: public Shape {
public:
    std::vector<Point> vertices;
    size_t n = 0;

    Polygon() = default;

    Polygon(const std::vector<Point>& init_verts)
        : vertices(init_verts), n(init_verts.size()) {
    }

    template<typename... Args>
    Polygon(const Point& point1, Args... points)
        : Polygon(points...) {
        vertices.push_back(point1);
        ++n;
    }

    Polygon(const Polygon& orig)
        :vertices(orig.vertices), n(orig.n) {
    }

    size_t verticesCount() const {
        return n;
    }

    const std::vector<Point> &getVertices() const {
        return vertices;
    }

    bool isConvex() {
        bool sign = getAbsVecProd(vertices[1] - vertices[0], vertices[2] - vertices[1]) > 0;
        for (size_t i = 1; i < vertices.size(); ++i) {
            if ((getAbsVecProd(vertices[(i+1) % n] - vertices[i],
                               vertices[(i+2) % n] - vertices[(i+1) % n]) > 0) != sign)
                return false;
        }
        return true;
    }

    double perimeter() const override {
        double res = 0;
        for (size_t i = 0; i < vertices.size(); ++i) {
            res += (vertices[(i + 1) % n] - vertices[i]).abs();
        }
        return res;
    }

    double area() const override {
        double res = 0;
        for (size_t i = 0; i < n; ++i)
            res += vertices[i].x * (vertices[(i + 1) % n].y - vertices[(i + n - 1) % n].y);
        return 0.5 * std::abs(res);
    }

    bool operator==(const Shape& another) const override {
        const Polygon* another_pol = dynamic_cast<const Polygon*>(&another);
        if (another_pol == nullptr)
            return false;
        if (n != another_pol->n)
            return false;
        for (size_t i = 0; i < n; ++i) {
            bool success = true;
            for (size_t j = 0; j < n; ++j) {
                if (vertices[(i + j) % n] != another_pol->vertices[j]) {
                    success = false;
                    break;
                }
            }
            if (success) return true;
        }
        for (size_t i = 0; i < n; ++i) {
            bool success = true;
            for (size_t j = 0; j < n; ++j) {
                if (vertices[(i + j) % n] != another_pol->vertices[n - j - 1]) {
                    success = false;
                    break;
                }
            }
            if (success) return true;
        }
        return false;
    }

protected:
    static bool coincide(double x1, double x2, double x3, double x4) {
        if (x1 > x2)
            std::swap(x1, x2);
        if (x3 > x4)
            std::swap(x3, x4);
        return std::max(x1, x3) - std::min(x2, x4) <= EPS;
    }

    static int area_for_intersect (const Point& a, const Point& b, const Point& c) {
        auto res = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (res > EPS)
            return 1;
        else if (res > -EPS)
            return 0;
        else
            return -1;
    }

    static bool intersect(const Point& a, const Point& b, const Point& c, const Point& d) {
        return coincide(a.x, b.x, c.x, d.x)
               && coincide(a.y, b.y, c.y, d.y)
               && area_for_intersect(a, b, c) * area_for_intersect(a, b, d) <= 0
               && area_for_intersect(c, d, a) * area_for_intersect(c, d, b) <= 0;
    }

    static double dist(double x0, double y0, double x1, double y1, double x2, double y2) {
        double cos1 = ((x0 - x1)*(x2-x1) + (y0-y1)*(y2-y1)) /
                      (sqrt((x0 - x1)*(x0 - x1) + (y0-y1)*(y0-y1)) * sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
        double cos2 = ((x0 - x2)*(x1-x2) + (y0-y2)*(y1-y2)) /
                      (sqrt((x0 - x2)*(x0 - x2) + (y0-y2)*(y0-y2)) * sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
        double d;
        if (cos1 >= 0 && cos2 >= 0) {
            double a, b, c;
            a = y1 - y2;
            b = x1 - x2;
            c = x1*y2 - x2*y1;
            d = std::abs(a * x0 - b * y0 + c) /
                sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
        }
        else
            d = std::min(sqrt(pow(x1-x0, 2) + pow(y1-y0, 2)),
                         sqrt(pow(x2-x0, 2) + pow(y2-y0, 2)));
        return d;
    }

public:
    bool containsPoint(const Point& point) const override {
        const double INF = 1e18;
        size_t crosses = 0;
        int ans = -1;
        for (size_t i = 1; i < n + 1; ++i) {
            double d = dist(point.x, point.y, vertices[i-1].x, vertices[i-1].y, vertices[i % n].x, vertices[i % n].y);
            if (d < EPS) {
                ans = true;
                break;
            }
            crosses += intersect(point, Point(INF, point.y + 10000), vertices[i-1], vertices[i % n]);
        }
        if (ans == -1)
            ans = crosses % 2;
        return ans;
    }

    void rotate(const Point& center, double angle) override {
        for (size_t i = 0; i < n; ++i)
            vertices[i].rotate(center, angle);
    }

    void reflect(const Point& center) override {
        for (size_t i = 0; i < n; ++i)
            vertices[i].reflect(center);
    }

    void reflect(const Line& axis) override {
        for (size_t i = 0; i < n; ++i)
            vertices[i].reflect(axis);
    }

    void scale(const Point& center, double coeff) override {
        for (size_t i = 0; i < n; ++i)
            vertices[i].scale(center, coeff);
    }

    Point center() const {
        auto center = Point(0, 0);
        for (size_t i = 0; i < n; ++i)
            center += vertices[i];
        return center /= n;
    }

    bool isCongruentTo(const Shape& another) const override {
        const Polygon* another_pol = dynamic_cast<const Polygon*>(&another);
        if (another_pol == nullptr)
            return false;
        if (n != another_pol->n)
            return false;
        for (size_t i = 0; i < n; ++i) {
            bool success = true;
            for (size_t j = 0; j < n; ++j) {
                if (std::abs((vertices[(i+j)%n] - vertices[(i+j+1)%n]).abs()
                             - (another_pol->vertices[j] - another_pol->vertices[(j + 1) % n]).abs()) > EPS)
                    success = false;
            }
            if (success)
                return true;
        }
        for (size_t i = 0; i < n; ++i) {
            bool success = true;
            for (size_t j = 0; j < n; ++j) {
                if (std::abs((vertices[(i+j)%n] - vertices[(i+j+1)%n]).abs()
                             - (another_pol->vertices[n - j - 1] - another_pol->vertices[(2*n - j - 2) % n]).abs()) > EPS)
                    success = false;
            }
            if (success)
                return true;
        }
        return false;
    }

    bool isSimilarTo(const Shape& another) const override {
        const Polygon* another_pol = dynamic_cast<const Polygon*>(&another);
        if (another_pol == nullptr)
            return false;
        if (n != another_pol->n)
            return false;
        for (size_t i = 0; i < n; ++i) {
            bool success = true;
            double k = (vertices[i] - vertices[(i+1)%n]).abs()
                       / (another_pol->vertices[0] - another_pol->vertices[1]).abs();
            for (size_t j = 0; j < n; ++j) {
                if (std::abs((vertices[(i+j)%n] - vertices[(i+j+1)%n]).abs()
                             / (another_pol->vertices[j] - another_pol->vertices[(j + 1) % n]).abs() - k) > EPS) {
                    success = false;
                    break;
                }
            }
            if (success)
                return true;
        }
        for (size_t i = 0; i < n; ++i) {
            bool success = true;
            double k = (vertices[i] - vertices[(i+1)%n]).abs()
                       / (another_pol->vertices[n - 1] - another_pol->vertices[n - 2]).abs();
            for (size_t j = 0; j < n; ++j) {
                if (std::abs((vertices[(i+j)%n] - vertices[(i+j+1)%n]).abs()
                             / (another_pol->vertices[n - j - 1] - another_pol->vertices[(2*n - j - 2) % n]).abs() - k) > EPS) {
                    success = false;
                    break;
                }
            }
            if (success)
                return true;
        }
        return false;
    }
};

class Ellipse: public Shape {
public:
    Point f1, f2;
    double a;

    Ellipse(const Point& f1, const Point& f2, double _2a)
        : f1(f1), f2(f2), a(_2a / 2) {}

protected:
    double _c() const {
        return (f1 - f2).abs() / 2;
    }

    double _b() const {
        auto c = _c();
        return sqrt(a * a - c * c);
    }

public:
    std::pair<Point,Point> focuses() const {
        return {f1, f2};
    }

    double eccentricity() const {
        return _c() / a;
    }

    Point center() const {
        return (f1 + f2) / 2;
    }

    std::pair<Line, Line> directrices() const {
        auto d = a / eccentricity();
        auto n = Vector((f1 - f2).y, -(f1 - f2).x);
        auto vec = Vector(f1 - f2);
        auto T1 = center() + vec * d;
        auto T2 = center() - vec * d;
        return {Line(T1, T1 + n), Line(T2, T2 + n)};
    }

    double perimeter() const override {
        auto b = _b();
        auto d = (a - b) * (a - b) / (a + b) / (a + b);
        return M_PI * (a + b) * (1 + 3 * d / (10 + sqrt(4 - 3 * d)));
    }

    double area() const override {
        return M_PI * a * _b();
    }

    bool containsPoint(const Point& point) const override {
        return (f1 - point).abs() + (f2 - point).abs() - 2 * a < EPS;
    }

    void rotate(const Point& center, double angle) override {
        f1.rotate(center, angle);
        f2.rotate(center, angle);
    }

    void reflect(const Point& center) override {
        f1.reflect(center);
        f2.reflect(center);
    }

    void reflect(const Line& axis) override {
        f1.reflect(axis);
        f2.reflect(axis);
    }

    void scale(const Point& center, double coeff) override {
        f1.scale(center, coeff);
        f2.scale(center, coeff);
        a *= std::abs(coeff);
    }

    bool operator==(const Shape& another) const override {
        const Ellipse* another_ell = dynamic_cast<const Ellipse*>(&another);
        if (another_ell == nullptr)
            return false;
        return ((f1 == another_ell->f1 && f2 == another_ell->f2)
               || (f1 == another_ell->f2 && f2 == another_ell->f1))
               && std::abs(a - another_ell->a) < EPS;
    }

    bool isCongruentTo(const Shape& another) const override {
        if (*this == another)
            return true;
        const Ellipse* another_ell = dynamic_cast<const Ellipse*>(&another);
        if (another_ell == nullptr)
            return false;
        return a == another_ell->a && std::abs((f1 - f2).abs() - (another_ell->f1 - another_ell->f2).abs()) < EPS;
    }

    bool isSimilarTo(const Shape& another) const override {
        if (isCongruentTo(another))
            return true;
        const Ellipse* another_ell = dynamic_cast<const Ellipse*>(&another);
        if (another_ell == nullptr)
            return false;
        return std::abs(a / another_ell->a - (f1 - f2).abs() / (another_ell->f1 - another_ell->f2).abs()) < EPS;
    }
};

class Circle: public Ellipse {
public:
    Circle(const Point& center, double radius)
        : Ellipse(center, center, 2 * radius) {}

    double radius() const {
        return Ellipse::a;
    }
};

class Rectangle: public Polygon {
public:
    Rectangle(const Point& p1, const Point& p2, double k)
        :Polygon(p1, p1, p2, p2) {
        if (k > 1)
            k = 1 / k;
        auto alpha = atan(k) * 180 / M_PI;
        auto dP = p2 - p1;
        auto v1 = Vector(dP).rotate(Point(0, 0), -alpha);
        auto v2 = Vector(-dP).rotate(Point(0, 0), -alpha);
        auto p3 = p1 + (Vector(dP, false) * v1) * v1;
        auto p4 = p2 + (Vector(-dP, false) * v2) * v2;
        Polygon::vertices[0] = p4;
        Polygon::vertices[2] = p3;
    }
    Point center() const {
        return (Polygon::vertices[0] + Polygon::vertices[2]) / 2;
    }

    std::pair<Line, Line> diagonals() const {
        return std::pair<Line, Line>(Line(Polygon::vertices[0], Polygon::vertices[2]),
                                     Line(Polygon::vertices[1], Polygon::vertices[3]));
    }
};

class Square: public Rectangle {
public:
    Square(const Point& p1, const Point& p2)
        : Rectangle::Rectangle(p1, p2, 1) {}

protected:
    double _a() const {
        return (Rectangle::vertices[0] - Rectangle::vertices[1]).abs();
    }

public:
    Circle circumscribedCircle() const {
        return Circle(center(), _a() / sqrt(2));
    }

    Circle inscribedCircle() const {
        return Circle(center(), _a() / 2);
    }
};

class Triangle: public Polygon {
private:
    using Polygon::vertices;
    double a, b, c;
    double x1 = vertices[0].x, x2 = vertices[1].x, x3 = vertices[2].x;
    double y1 = vertices[0].y, y2 = vertices[1].y, y3 = vertices[2].y;

public:
    Triangle(const Point& p1, const Point& p2, const Point& p3)
        : Polygon::Polygon(p1, p2, p3),
          a((p1 - p2).abs()), b((p1 - p3).abs()), c((p2 - p3).abs()) {}

private:
    Point circumscribedCenter() const {
        auto denum = x1 * y2 + y1 * x3 + x2 * y3 - y2 * x3 - x1 * y3 - y1 * x2;
        auto x0 = y2 * (x1 * x1 + y1 * y1) + y1 * (x3 * x3 + y3 * y3) + y3 * (x2 * x2 + y2 * y2)
                  - y2 * (x3 * x3 + y3 * y3) - y3 * (x1 * x1 + y1 * y1) - y1 * (x2 * x2 + y2 * y2);
        auto y0 = x1 * (x2 * x2 + y2 * y2) + x3 * (x1 * x1 + y1 * y1) + x2 * (x3 * x3 + y3 * y3)
                  - x3 * (x2 * x2 + y2 * y2) - x1 * (x3 * x3 + y3 * y3) - x2 * (x1 * x1 + y1 * y1);
        return Point(x0, y0) / (2 * denum);
    }

public:
    Circle circumscribedCircle() const {
        auto center = circumscribedCenter();
        auto r = (center - vertices[0]).abs();
        return Circle(center, r);
    }

    Circle inscribedCircle() const {
        auto biss_center = Point(a * x1 + b * x2 + c * x3, a * y1 + b * y2 + c * y3) / (a + b + c);
        auto r = 2 * Polygon::area() / (a + b + c);
        return Circle(biss_center, r);
    }

    Point centroid() const {
        return Point((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3);
    }

    Point orthocenter() const {
        double denum = x1 * y2 + y1 * x3 + x2 * y3 - y2 * x3 - x1 * y3 - y1 * x2;
        double x0 = y1 * (x3 * x1 + y2 * y2) + y3 * (x2 * x3 + y1 * y1) + y2 * (x1 * x2 + y3 * y3)
                     - y3 * (x3 * x1 + y2 * y2) - y1 * (x1 * x2 + y3 * y3) - y2 * (x2 * x3 + y1 * y1);
        double y0 = x2 * (x1 * x1 + y2 * y3) + x1 * (x3 * x3 + y1 * y2) + x3 * (x2 * x2 + y3 * y1)
                     - x2 * (x3 * x3 + y1 * y2) - x3 * (x1 * x1 + y2 * y3) - x1 * (x2 * x2 + y3 * y1);
        return Point(x0, y0) / denum;
    }

    Line EulerLine() const {
        return Line(circumscribedCenter(), orthocenter());
    }

    Circle ninePointsCircle() const {
        auto center = (circumscribedCenter() + orthocenter()) / 2;
        auto r = circumscribedCircle().radius() / 2;
        return Circle(center, r);
    }
};
