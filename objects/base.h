# ifndef __BASE_H__
# define __BASE_H__

# include <cassert>
# include <cmath>
# include <iostream>

# include "../utils.h"

class Vector3D {
public:
    double x, y, z;
    Vector3D(double _x=0, double _y=0, double _z=0): x(_x), y(_y), z(_z) {}

    inline Vector3D operator - () const { return Vector3D(-x, -y, -z); }
    inline Vector3D operator + (const Vector3D &b) const { return Vector3D(x + b.x, y + b.y, z + b.z); }
    inline Vector3D operator - (const Vector3D &b) const { return Vector3D(x - b.x, y - b.y, z - b.z); }
    inline Vector3D operator * (double c) const { return Vector3D(x * c, y * c, z * c); }
    inline Vector3D operator / (double c) const { return Vector3D(x / c, y / c, z / c); }
    inline Vector3D& operator += (const Vector3D &b) { return *this = *this + b; }
    inline Vector3D& operator -= (const Vector3D &b) { return *this = *this - b; }
    inline Vector3D& operator /= (double c) { return *this = *this / c; }
    inline Vector3D& operator *= (double c) { return *this = *this * c; }

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

    inline int r() { return gray2int(x); }
    inline int g() { return gray2int(y); }
    inline int b() { return gray2int(z); }
    inline Vector3D clamp() { return Vector3D(::clamp(x), ::clamp(y), ::clamp(z)); }

    void print() {
        std:: cout << "Vector3D: " << x << ", " << y << ", " << z << std:: endl;
        return;
    }
};

typedef Vector3D Color_F;

class Ray {
public:
    Vector3D o, d;
    Ray(Vector3D _o, Vector3D _d): o(_o), d(_d) {}
};

class Range {
public:
    Vector3D l, r;
    Range(Vector3D _l, Vector3D _r): l(_l), r(_r) {}
};

enum ReflectType { DIFF, SPEC, REFR};
typedef unsigned char Color_U;

# endif