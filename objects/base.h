# ifndef __BASE_H__
# define __BASE_H__

# include <cassert>
# include <cmath>
# include <iostream>

# include "../utils.h"

class Vector3D {
public:
    double x, y, z;
    Vector3D(double _x=0, double _y=0, double _z=0): x(_x), y(_y), z(_z) { }

    inline Vector3D operator - () const { return Vector3D(-x, -y, -z); }
    inline Vector3D operator + (const Vector3D &b) const { return Vector3D(x + b.x, y + b.y, z + b.z); }
    inline Vector3D operator - (const Vector3D &b) const { return Vector3D(x - b.x, y - b.y, z - b.z); }
    inline Vector3D operator * (double c) const { return Vector3D(x * c, y * c, z * c); }
    inline Vector3D operator / (double c) const { return Vector3D(x / c, y / c, z / c); }
    inline Vector3D& operator += (const Vector3D &b) { return *this = *this + b; }
    inline Vector3D& operator -= (const Vector3D &b) { return *this = *this - b; }
    inline Vector3D& operator /= (double c) { return *this = *this / c; }
    inline Vector3D& operator *= (double c) { return *this = *this * c; }
    inline Vector3D operator + (double c) { return Vector3D(x + c, y + c, z + c); }
    inline Vector3D operator - (double c) { return Vector3D(x - c, y - c, z - c); }
    inline bool operator <= (const Vector3D &b) const { return (x <= b.x && y <= b.y && z <= b.z); }

    inline Vector3D norm() const { return (*this) / length(); }
    inline double max() const { return std:: max(std:: max(x, y), z); }
    inline double length() const { return sqrt(x * x + y * y + z * z); }
    inline double length2() const { return x * x + y * y + z * z; }
    inline double dot(const Vector3D &b) const { return x * b.x + y * b.y + z * b.z; }
    inline Vector3D mul(const Vector3D &b) const { return Vector3D(x * b.x, y * b.y, z * b.z); }
    inline Vector3D cross(const Vector3D &b) const { return Vector3D(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }
    inline Vector3D reflect(const Vector3D &n) const { return (*this) - n * 2. * dot(n); }
    inline Vector3D refract(const Vector3D n, double n_i, double n_r) const {
        double cos_i = dot(n), n_ir = n_i / n_r, cos2_r = 1. - n_ir * n_ir * (1 - cos_i * cos_i);
        if(cos2_r <= 0) return Vector3D();
        return ((*this) * n_ir - n * (n_ir * cos_i + sqrt(cos2_r))).norm();
    }
    inline void div(const Vector3D &cx, const Vector3D &cy, double &x, double &y) const {
        x = dot(cx) / cx.dot(cx);
        y = dot(cy) / cy.dot(cy);
        return;
    }

    inline friend std:: ostream& operator << (std:: ostream &os, const Vector3D &v) { os << "(" << v.x << ", " << v.y << ", " << v.z << ")"; return os; }
    inline double dim(int d) const { return d == 0 ? x : (d == 1 ? y : z); }
    inline double& dim_addr(int d) { return d == 0 ? x : (d == 1 ? y : z); }
    inline int r() const { return gray2int(x); }
    inline int g() const { return gray2int(y); }
    inline int b() const { return gray2int(z); }
    inline Vector3D clamp() const { return Vector3D(::clamp(x), ::clamp(y), ::clamp(z)); }
    inline void expand_min(const Vector3D &b) {
        if(x > b.x) x = b.x;
        if(y > b.y) y = b.y;
        if(z > b.z) z = b.z;
        return;
    }

    inline void expand_max(const Vector3D &b) {
        if(x < b.x) x = b.x;
        if(y < b.y) y = b.y;
        if(z < b.z) z = b.z;
        return;
    }

    void print() const {
        std:: cout << "Vector3D: " << x << ", " << y << ", " << z << std:: endl;
        return;
    }
};

class Vector2D {
public:
    double x, y;
    Vector2D(double _x=0, double _y=0): x(_x), y(_y) { }

    inline Vector2D operator - () const { return Vector2D(-x, -y); }
    inline Vector2D operator + (const Vector2D &b) const { return Vector2D(x + b.x, y + b.y); }
    inline Vector2D operator - (const Vector2D &b) const { return Vector2D(x - b.x, y - b.y); }
    inline Vector2D operator * (double c) const { return Vector2D(x * c, y * c); }
    inline Vector2D operator / (double c) const { return Vector2D(x / c, y / c); }
    inline Vector2D& operator += (const Vector2D &b) { return *this = *this + b; }
    inline Vector2D& operator -= (const Vector2D &b) { return *this = *this - b; }
    inline Vector2D& operator /= (double c) { return *this = *this / c; }
    inline Vector2D& operator *= (double c) { return *this = *this * c; }

    inline double& dim_addr(int d) { return d == 0 ? x : y; }
    inline Vector2D norm() const { return (*this) / length(); }
    inline double length() const { return sqrt(x * x + y * y); }
    inline double length2() const { return x * x + y * y; }
    inline void print() const {
        std:: cout << "Vector2D: " << x << ", " << y << std:: endl;
        return;
    }
};

typedef Vector3D Color_F;

class Ray {
public:
    Vector3D o, d;
    Ray(Vector3D _o, Vector3D _d): o(_o), d(_d) {}
    void print() const { o.print(); d.print(); return; }
};

class Range1D {
public:
    double l, r;
    Range1D(double _l, double _r): l(_l), r(_r) { }
    
    void print() const { std:: cout << "range: " << l << " " << r << std:: endl; return; }
    inline bool in_range(double t) const { return l <= t && t <= r; }
};

class Range3D {
public:
    Vector3D l, r;
    Range3D() { }
    Range3D(Vector3D _l, Vector3D _r): l(_l), r(_r) {}

    inline void print() const { l.print(); r.print(); return; }
    inline void overstate(const Vector3D &pos, double radius) {
        l = Vector3D(pos.x - radius, pos.y - radius, pos.z - radius);
        r = Vector3D(pos.x + radius, pos.y + radius, pos.z + radius);
        return;
    }
    inline void surface(Vector3D *points) {
        l = r = points[0];
        l.expand_min(points[1]), l.expand_min(points[2]);
        r.expand_max(points[1]), r.expand_max(points[2]);
        return;
    }
    inline void expand(const Range3D &range) {
        l.expand_min(range.l);
        r.expand_max(range.r);
        return;
    }
    inline bool contain(const Vector3D &pos) {
        return l <= pos && pos <= r;
    }

    inline double estimate(const Ray &ray) {
        double x = 1. / ray.d.x, y = 1. / ray.d.y, z = 1. / ray.d.z;
        double t1 = (l.x - ray.o.x) * x, t2 = (r.x - ray.o.x) * x;
        double t3 = (l.y - ray.o.y) * y, t4 = (r.y - ray.o.y) * y;
        double t5 = (l.z - ray.o.z) * z, t6 = (r.z - ray.o.z) * z;
        double tmin = std:: max(std:: max(std:: min(t1, t2), std:: min(t3, t4)), std:: min(t5, t6));
        double tmax = std:: min(std:: min(std:: max(t1, t2), std:: max(t3, t4)), std:: max(t5, t6));
        if(tmax < 0 || tmin > tmax) return inf;
        return tmin < 0 ? tmax : tmin;
    }
};

enum ReflectType { DIFF, SPEC, REFR };
typedef unsigned char Color_U;

struct Pixel {
    Color_F color; double count, r;
    Pixel(Color_F _color=Color_F(), int _count=0, double _r=0): color(_color), count(_count), r(_r) { }

    inline Pixel operator + (const Pixel &b) { return Pixel(color + b.color, count + b.count); }
    inline void add(const Color_F &light) {
        count += 1.0, color += light;
        return;
    }
    inline Color_F value() {
        return count ? (color / (count * M_PI * sqr(r))) : Color_F();
    }
    inline void print() {
        std:: cout << color << " / " << count << std:: endl;
        return;
    }
    inline void modify(const Pixel &n, double alpha) {
        double r_count = count, r_r = r;
        count = count + n.count * alpha;
        r = r * sqrt(count / (r_count + n.count));
        color = (color + n.color) * sqr(r / r_r);
        return;
    }
};

# endif