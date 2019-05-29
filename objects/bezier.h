# ifndef __BEZIER_H__
# define __BEZIER_H__

# include "base.h"
# include "object.h"

class Equation3D {
public:
    Vector2D a, b, c, d;
    Equation3D(Vector2D _a=Vector2D(), Vector2D _b=Vector2D(), Vector2D _c=Vector2D(), Vector2D _d=Vector2D()): a(_a), b(_b), c(_c), d(_d) { }

    double solve(const Vector2D &pos, const Vector2D &dir) {
        double t, na, nb, nc, nd;
        na = a.x * dir.y - a.y * dir.x;
        nb = b.x * dir.y - b.y * dir.x;
        nc = c.x * dir.y - c.y * dir.x;
        nd = d.x * dir.y - d.y * dir.x - pos.x * dir.y + pos.y * dir.x;
        /* Now the problem is to solve: na * t^3 + nb * t^2 + nc * t + nd = 0 */
        
        return t;
    }
};

class Bezier2D {
public:
    int n_points;
    Vector2D *points;
    Equation3D *equations;
    Bezier2D(int _n_points, Vector2D *_points): n_points(_n_points), points(_points), equations(nullptr) { }
    ~Bezier2D() {
        if(equations != nullptr) {
            std:: free(equations);
        }
        return;
    }

    void get_control_points(const Vector2D &la, const Vector2D &no, const Vector2D &nx, Vector2D &c1, Vector2D &c2) {
        double l1 = (no - la).length(), l2 = (nx - no).length();
        c1 = (la + no) / 2., c2 = (no + nx) / 2.;
        Vector2D b = c1 * (l2 / (l1 + l2)) + c2 * (l1 / (l1 + l2));
        c1 += no - b, c2 += no - b;
        return;
    }

    /*
     * B(t) = P_0 * (1 - t)^3 + 3 * P_1 * t * (1 - t)^2 + 3 * P_2 * t^2 * (1 - t) + P_3 * t^3
     * P_0: (1 - t)^3         = -t^3 + 3 * t^2 - 3 * t + 1
     * P_1: 3 * t * (1 - t)^2 = 3t^3 - 6 * t^2 + 3 * t
     * P_2: 3 * t^2 * (1 - t) = -t^3 + t^2
     * P_3: t^3               =  t^3
     * t: 0 -> 1
     */
    void calc_beizer(const Vector2D &p0, const Vector2D &p1, const Vector2D &p2, const Vector2D &p3, Equation3D &equation) {
        equation.a = -p0 + p1 * 3 - p2 + p3;
        equation.b = p0 * 3 - p1 * 6 + p2;
        equation.c = p0 * (-3) + p1 * 3;
        equation.d = 1;
        return;
    }

    double solve(const Vector2D &o, const Vector2D &d) {
        double t = inf, tmp;
        for(int i = 0; i < n_points; ++ i) {
            tmp = equations[i].solve(o, d);
            if(tmp > eps && tmp < t) t = tmp;
        }
        return t;
    }

    void preprocess() {
        Vector2D *lc = (Vector2D*) std:: malloc(sizeof(Vector2D) * n_points);
        Vector2D *nc = (Vector2D*) std:: malloc(sizeof(Vector2D) * n_points);
        for(int i = 0; i < n_points; ++ i) {
            int la = (i - 1 + n_points) % n_points;
            int nx = (i + 1) % n_points;
            get_control_points(points[la], points[i], points[nx], lc[i], nc[i]);
        }
        equations = (Equation3D*) std:: malloc(sizeof(Equation3D) * n_points);
        for(int i = 0; i < n_points; ++ i) {
            int la = (i - 1 + n_points) % n_points;
            int nx = (i + 1) % n_points;
            calc_beizer(points[i], nc[i], lc[nx], points[nx], equations[i]);
        }
        std:: free(lc), std:: free(nc);
        return;
    }
};

/* Bezier2D(x, z), y is free */
class Bezier3D: public Object {
public:
    Bezier2D bezier2d; Range1D range_y;

    Bezier3D(Bezier2D _bezier2d, Range1D _range_y, ReflectType _reflect, double _ior, Texture _texture, Color_F _emission):
        bezier2d(_bezier2d), range_y(_range_y), Object(_reflect, _ior, _texture, _emission) { }
    ~Bezier3D() { }

    void preprocess() {
        bezier2d.preprocess();
        return;
    }

    virtual double intersect(const Ray &ray) {
        double t = bezier2d.solve(Vector2D(ray.o.x, ray.o.z), Vector2D(ray.d.x, ray.d.z));
        return range_y.in_range(ray.o.y + t * ray.d.y) ? t : 0;
    }
};

# endif
