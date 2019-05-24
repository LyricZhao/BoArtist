# ifndef __BASE_H__
# define __BASE_H__

# include <cmath>
# include <iostream>

# include "../utils.h"

# define eps 1e-4

class Vector3D {
public:
    double x, y, z;
    Vector3D(double _x=0, double _y=0, double _z=0): x(_x), y(_y), z(_z) {}

    Vector3D operator + (const Vector3D &b) { return Vector3D(x + b.x, y + b.y, z + b.z); }
    Vector3D operator - (const Vector3D &b) { return Vector3D(x - b.x, y - b.y, z - b.z); }
    Vector3D operator * (double c) { return Vector3D(x * c, y * c, z * c); }
    Vector3D operator / (double c) { return Vector3D(x / c, y / c, z / c); }
    Vector3D& operator += (const Vector3D &b) { return *this = *this + b; }
    Vector3D& operator -= (const Vector3D &b) { return *this = *this - b; }
    Vector3D& operator *= (double c) { return *this = *this * c; }
    Vector3D& operator /= (double c) { return *this = *this / c; }

    Vector3D norm() { return (*this) / length(); }
    double length() { return sqrt(x * x + y * y + z * z); }
    double dot(const Vector3D &b) { return x * b.x + y * b.y + z * b.z; }
    Vector3D cross(const Vector3D &b) { return Vector3D(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }

    int r() { return gray2int(x); }
    int g() { return gray2int(y); }
    int b() { return gray2int(z); }
    Vector3D clamp() { return Vector3D(::clamp(x), ::clamp(y), ::clamp(z)); }

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

enum ReflectType { DIFF, SPEC, REFR};
typedef unsigned int Color_U;

# endif