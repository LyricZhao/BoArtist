# ifndef __CUBE_H__
# define __CUBE_H__

# include "base.h"
# include "object.h"

class Cube: public Object {
public:
    Vector3D o, x, y, z, ux, uy, uz;

    Cube(Vector3D _o, Vector3D _x, Vector3D _y, Vector3D _z, ReflectType _reflect, double _ior, Texture _texture, Color_F _emission):
        o(_o), x(_x), y(_y), z(_z), Object(_reflect, _ior, _texture, _emission) { ux = x - o, uy = y - o, uz = z - o; }
    
    double cross_t(const Vector3D &a, const Vector3D &b, const Vector3D &c, const Ray &ray) const {
        Vector3D n = (b - a).cross(c - a);
        n /= n.dot(a);
        double t = ray.d.dot(n);
        if(t < 0) return 0;
        t = (1 - ray.o.dot(n)) / t;
        return t < 0 ? 0 : t;
    }

    virtual double intersect(const Ray &ray) const {
        double t = inf;
        update_solve(t, cross_t(o     , x     , y     , ray)); // down
        update_solve(t, cross_t(o + uz, x + uz, y + uz, ray)); // up
        update_solve(t, cross_t(o     , x     , z     , ray)); // front
        update_solve(t, cross_t(o + uy, x + uy, z + uy, ray)); // back
        update_solve(t, cross_t(o     , z     , y     , ray)); // left
        update_solve(t, cross_t(o + ux, z + ux, y + ux, ray)); // right
        return t;
    }
};

# endif