# ifndef __SPHERE_H__
# define __SPHERE_H__

# include "base.h"

struct Sphere {
    double r, n_s;
    Vector3D pos;
    Color_F emission, color;
    ReflectType reflect;

    Sphere(double _r, Vector3D _pos, Color_F _emission, Color_F _color, ReflectType _reflect, double _n_s=1.5):
        r(_r), n_s(_n_s), pos(_pos), emission(_emission), color(_color), reflect(_reflect) { }
    inline double intersect(const Ray &ray) {
        Vector3D z = pos - ray.o;
        double b, det, t;
        b = z.dot(ray.d); det = b * b + r * r - z.length2();
        if(det < 0) return 0;
        det = sqrt(det);
        return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
    }
};

# endif