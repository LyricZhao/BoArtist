# ifndef __SPHERE_H__
# define __SPHERE_H__

# include "utils.h"

struct Sphere {
    double r;
    Vector3D pos;
    Color_F emission, color;
    ReflectType reflect;

    Sphere(double _r, Vector3D _pos, Color_F _emission, Color_F _color, ReflectType _reflect):
        r(_r), pos(_pos), emission(_emission), color(_color), reflect(_reflect) { }
    double intersect(const Ray &ray) {
        Vector3D z = pos - ray.o;
        double b, det, t;
        b = z.dot(ray.d); det = b * b + r * r - z.dot(z);
        if(det < 0) return 0;
        det = sqrt(det);
        return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
    }
};

# endif