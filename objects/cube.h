# ifndef __CUBE_H__
# define __CUBE_H__

# include "base.h"
# include "object.h"

class Cube: public Object {
public:
    Vector3D o, x, y, z, ux, uy, uz, t_norm; bool has_norm;

    Cube(Vector3D _o, Vector3D _x, Vector3D _y, Vector3D _z, ReflectType _reflect, double _ior, Texture _texture, Color_F _emission):
        o(_o), x(_x), y(_y), z(_z), Object(_reflect, _ior, _texture, _emission) { ux = x - o, uy = y - o, uz = z - o; }
    
    void cross_t(double &ut, const Vector3D &a, const Vector3D &b, const Vector3D &c, const Ray &ray) {
        Vector3D n = (b - a).cross(c - a);
        n /= n.dot(a);
        double t = ray.d.dot(n);
        if(t < 0) return;
        t = (1 - ray.o.dot(n)) / t;
        if(t > 0 && t < ut) {
            ut = t, t_norm = n, has_norm = true;
        }
        return;
    }

    /* TODO */
    virtual Color_F color(const Vector3D &x) {
        return texture.color;
    }

    virtual Vector3D norm(const Vector3D &x) {
        assert(has_norm);
        return t_norm;
    }

    virtual double intersect(const Ray &ray) {
        double t = inf; has_norm = false;
        cross_t(t, o     , x     , y     , ray); // down
        cross_t(t, o + uz, x + uz, y + uz, ray); // up
        cross_t(t, o     , x     , z     , ray); // front
        cross_t(t, o + uy, x + uy, z + uy, ray); // back
        cross_t(t, o     , z     , y     , ray); // left
        cross_t(t, o + ux, z + ux, y + ux, ray); // right
        return t < inf ? t : 0;
    }
};

# endif