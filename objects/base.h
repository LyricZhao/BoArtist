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

    inline int r() const { return gray2int(x); }
    inline int g() const { return gray2int(y); }
    inline int b() const { return gray2int(z); }
    inline Vector3D clamp() const { return Vector3D(::clamp(x), ::clamp(y), ::clamp(z)); }

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

    inline double length() const { return sqrt(x * x + y * y); }
    inline double length2() const { return x * x + y * y; }
    inline void print() const {
        std:: cout << "Vector2D:" << x << ", " << y << std:: endl;
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
 
    inline bool in_range(double t) const { return l <= t && t <= r; }
};

class Range3D {
public:
    Vector3D l, r;
    Range3D(Vector3D _l, Vector3D _r): l(_l), r(_r) {}
    void print() const { l.print(); r.print(); return; }
};

enum ReflectType { DIFF, SPEC, REFR};
typedef unsigned char Color_U;

# endif