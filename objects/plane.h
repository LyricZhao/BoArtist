# ifndef __PLANE_H__
# define __PLANE_H__

# include "base.h"
# include "object.h"

class Plane: public Object {
public:
    Vector3D n, o, cx, cy;

    Plane(Vector3D _n, ReflectType _reflect, double _ior, Texture _texture, Color_F _emission, Vector3D _o=Vector3D(), Vector3D _cx=Vector3D(), Vector3D _cy=Vector3D()):
        n(_n), Object(_reflect, _ior, _texture, _emission), o(_o), cx(_cx), cy(_cy)  { }

    virtual double intersect(const Ray &ray, Vector3D &gn, Color_F &f) {
        double t = ray.d.dot(n);
        if(t < 0) return 0;
        t = (1 - ray.o.dot(n)) / t;
        gn = n.norm();
        if(t > 0) {
            f = color(ray.o + ray.d * t);
            return t;
        }
        return 0;
    }

    virtual Color_F color(const Vector3D &x) {
        if(texture.none()) return texture.color;
        double dx, dy; (x - o).div(cx, cy, dx, dy);
        return texture.get(dx, dy);
    }
};

# endif