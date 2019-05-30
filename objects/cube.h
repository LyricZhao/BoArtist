# ifndef __CUBE_H__
# define __CUBE_H__

# include "base.h"
# include "object.h"

class Cube: public Object {
public:
    Vector3D o, x, y, z, ux, uy, uz, norms[6];
    Vector3D range_l, range_r;

    Cube(Vector3D _o, Vector3D _x, Vector3D _y, Vector3D _z, ReflectType _reflect, double _ior, Texture _texture, Color_F _emission):
        o(_o), x(_x), y(_y), z(_z), range_l(Vector3D(inf, inf, inf)), range_r(Vector3D(-inf, -inf, -inf)), Object(_reflect, _ior, _texture, _emission) {
            ux = x - o, uy = y - o, uz = z - o;
            norms[0] = uy.cross(ux).norm(), norms[1] = ux.cross(uy).norm();
            norms[2] = ux.cross(uz).norm(), norms[3] = uz.cross(ux).norm();
            norms[4] = uz.cross(uy).norm(), norms[5] = uy.cross(uz).norm();
            upd_mnx(o), upd_mnx(x), upd_mnx(y), upd_mnx(z);
            upd_mnx(x + uy), upd_mnx(x + uz), upd_mnx(y + uz), upd_mnx(x + uy + uz);
            return;
    }

    void upd_mnx(const Vector3D &v) {
        upd_min(range_l.x, v.x), upd_max(range_r.x, v.x);
        upd_min(range_l.y, v.y), upd_max(range_r.y, v.y);
        upd_min(range_l.z, v.z), upd_max(range_r.z, v.z);
        return;
    }
    
    void cross_t(double &ut, const Vector3D &n, const Vector3D &a, const Ray &ray) {
        double t = ray.d.dot(n);
        if(fabs(t) < eps) return;
        ut = ((a - ray.o).dot(n)) / t;
        if(ut < 0) ut = 0;
        return;
    }

    /* TODO */
    virtual Color_F color(const Vector3D &x) {
        return texture.color;
    }

    bool judge_in_range(const Vector3D &v) {
        // range_l.print();
        // range_r.print();
        // v.print();
        return range_l - eps <= v && v <= range_r + eps;
    }

    /* Woo Method */
    virtual double intersect(const Ray &ray, Vector3D &gn) {
        // std:: cout << std:: endl;
        // ray.print();
        double t[6];
        t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = 0;
        cross_t(t[0], norms[0], o     , ray); // down
        cross_t(t[1], norms[1], o + uz, ray); // up
        cross_t(t[2], norms[2], o     , ray); // front
        cross_t(t[3], norms[3], o + uy, ray); // back
        cross_t(t[4], norms[4], o     , ray); // left
        cross_t(t[5], norms[5], o + ux, ray); // right
        double t_min_f = 0;
        for(int i = 0; i < 3; ++ i) {
            double v1 = t[i << 1], v2 = t[(i << 1) | 1];
            if(v1 > eps && v2 > eps) {
                upd_max(t_min_f, std:: min(v1, v2));
            }
        }
        if (t_min_f > eps && judge_in_range(ray.o + ray.d * t_min_f)) {
            for(int i = 0; i < 6; ++ i) if(fabs(t_min_f - t[i]) < eps) {
                gn = norms[i]; break;
            }
            return t_min_f;
        }
        // exit(0);
        return 0;
    }

    void print() {
        o.print(), x.print(), y.print(), z.print();
        ux.print(), uy.print(), uz.print();
        for(int i = 0; i < 6; ++ i) norms[i].print();
        range_l.print(), range_r.print();
        return;
    }
};

# endif