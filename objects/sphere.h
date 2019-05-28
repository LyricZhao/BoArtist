# ifndef __SPHERE_H__
# define __SPHERE_H__

# include "base.h"
# include "object.h"

class Sphere: public Object {
public:
    double r; Vector3D pos;

    Sphere(double _r, Vector3D _pos, double _ior, Texture _texture, ReflectType _reflect, Color_F _emission):
        r(_r), pos(_pos), Object(_reflect, _ior, _texture, _emission) { }

    virtual double intersect(const Ray &ray) const {
        Vector3D z = pos - ray.o;
        double b, det, t;
        b = z.dot(ray.d); det = b * b + r * r - z.length2();
        if(det < 0) return 0;
        det = sqrt(det);
        return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
    }

    virtual Vector3D norm(const Vector3D &x) const {
        return (x - pos).norm();
    }

    virtual Color_F color(const Vector3D &x) const {
        return texture.color;
    }
};

# endif