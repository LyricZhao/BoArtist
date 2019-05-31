# ifndef __BEZIER_H__
# define __BEZIER_H__

# include "base.h"
# include "object.h"

# define L_SQRT_3 1.7320508076

class Equation3D {
public:
    Vector2D a, b, c, d;
    Equation3D(Vector2D _a=Vector2D(), Vector2D _b=Vector2D(), Vector2D _c=Vector2D(), Vector2D _d=Vector2D()): a(_a), b(_b), c(_c), d(_d) { }

    inline void update_solve(double &ret, double u, const Ray &ray, const Range1D &range_y) {
        if(u < 0 || u > 1) return;
        double t;
        if(fabs(ray.d.x) > eps) {
            t = ((u * (u * (a.x * u + b.x) + c.x) + d.x) - ray.o.x) / ray.d.x;
        } else if(fabs(ray.d.z) > eps) {
            t = ((u * (u * (a.y * u + b.y) + c.y) + d.y) - ray.o.z) / ray.d.z;
        } else assert(0);
        if(t < 0 || t > ret) return;
        double y = ray.o.y + t * ray.d.y;
        if(range_y.in_range(y)) ret = t;
        return;
    }

    inline bool judge_solve(double u, const Vector2D &v) {
        if(u < 0 || u > 1) return false;
        double px = u * (u * (a.x * u + b.x) + c.x) + d.x;
        return px > v.x;
    }

    # define PREPROCESS_SHENGJIN double na_2 = na * na, na_3 = na_2 * na, p = nc / (3. * na) - nb * nb / (9. * na_2), q = nd / (2. * na) + nb * nb * nb / (27. * na_3) - nb * nc / (6. * na_2), delta = q * q + p * p * p
    # define PREPROCESS_CASE_3 double alpha = acos(-q * sqrt(-p) / (p * p)) / 3., sp_2 = 2. * sqrt(-p), cos_alpha = cos(alpha), sin_alpha = sin(alpha)

    # define ROOT_1_CASE_1 (pow(-q + sqrt(delta), 1. / 3) + pow(-q - sqrt(delta), 1. / 3))
    # define ROOT_1_CASE_2 (-2. * pow(q, 1. / 3))
    # define ROOT_2_CASE_2 (pow(q, 1. / 3))
    # define ROOT_1_CASE_3 (sp_2 * cos_alpha)
    # define ROOT_2_CASE_3 (sp_2 * (cos_alpha * (-1 / 2.) - sin_alpha * L_SQRT_3 / 2.))
    # define ROOT_3_CASE_3 (sp_2 * (cos_alpha * (-1 / 2.) + sin_alpha * L_SQRT_3 / 2.))

    int count(const Vector2D &v) {
        int tot = 0;
        /* Solve: a.y * t^3 + b.y * t^2 + c.y * t^3 + d.y = v.y, x > v.x */
        double na = a.y, nb = b.y, nc = c.y, nd = d.y - v.y;
        
        assert(fabs(na) > eps);
        PREPROCESS_SHENGJIN;
        if(delta > 0) {
            tot += judge_solve(ROOT_1_CASE_1, v);
        } else if(fabs(delta) < eps) {
            tot += judge_solve(ROOT_1_CASE_2, v);
            tot += judge_solve(ROOT_2_CASE_2, v);
        } else {
            PREPROCESS_CASE_3;
            tot += judge_solve(ROOT_1_CASE_3, v);
            tot += judge_solve(ROOT_2_CASE_3, v);
            tot += judge_solve(ROOT_3_CASE_3, v);
        }
        return tot;
    }

    double solve(const Ray &ray, const Range1D &range_y) {
        double ret = inf, na, nb, nc, nd;
        na = a.x * ray.d.z - a.y * ray.d.x;
        nb = b.x * ray.d.z - b.y * ray.d.x;
        nc = c.x * ray.d.z - c.y * ray.d.x;
        nd = d.x * ray.d.z - d.y * ray.d.x - ray.o.x * ray.d.z + ray.o.z * ray.d.x;
        /* Now the problem is to solve: na * t^3 + nb * t^2 + nc * t + nd = 0 */

        /* Shengjin Method */
        assert(fabs(na) > eps);
        PREPROCESS_SHENGJIN;
        if(delta > 0) {
            update_solve(ret, ROOT_1_CASE_1, ray, range_y);
        } else if(fabs(delta) < eps) {
            update_solve(ret, ROOT_1_CASE_2, ray, range_y);
            update_solve(ret, ROOT_2_CASE_2, ray, range_y);
        } else {
            PREPROCESS_CASE_3;
            update_solve(ret, ROOT_1_CASE_3, ray, range_y);
            update_solve(ret, ROOT_2_CASE_3, ray, range_y);
            update_solve(ret, ROOT_3_CASE_3, ray, range_y);
        }
        return ret < inf ? ret : 0;
    }

    void print() {
        printf("x: %.2lft^3 + %.2lft^2 + %.2lft + %.2lf\n", a.x, b.x, c.x, d.x);
        printf("z: %.2lft^3 + %.2lft^2 + %.2lft + %.2lf\n", a.y, b.y, c.y, d.y);
        return;
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
     * P_2: 3 * t^2 * (1 - t) = -3t^3 + 3t^2
     * P_3: t^3               =  t^3
     * t: 0 -> 1
     */
    void calc_beizer(const Vector2D &p0, const Vector2D &p1, const Vector2D &p2, const Vector2D &p3, Equation3D &equation) {
        equation.a = -p0 + p1 * 3 - p2 * 3 + p3;
        equation.b = p0 * 3 - p1 * 6 + p2 * 3;
        equation.c = p0 * (-3) + p1 * 3;
        equation.d = p0;
        return;
    }

    /* TODO: gn */
    double solve(const Ray &ray, const Range1D &range_y, Vector2D &gn) {
        double t = inf, tmp;
        for(int i = 0; i < n_points; ++ i) {
            tmp = equations[i].solve(ray, range_y);
            if(tmp > eps && tmp < t) t = tmp;
        }
        return t < inf ? t : 0;
    }

    bool in(const Vector2D &v) {
        int count = 0;
        for(int i = 0; i < n_points; ++ i)
            count += equations[i].count(v);
        return count & 1;
    }

    void preprocess() {
        Vector2D *lc = (Vector2D*) std:: malloc(sizeof(Vector2D) * n_points);
        Vector2D *nc = (Vector2D*) std:: malloc(sizeof(Vector2D) * n_points);
        std:: cout << "Control points:" << std:: endl;
        for(int i = 0; i < n_points; ++ i) {
            int la = (i - 1 + n_points) % n_points;
            int nx = (i + 1) % n_points;
            get_control_points(points[la], points[i], points[nx], lc[i], nc[i]);
            lc[i].print(), nc[i].print();
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

    void print() {
        for(int i = 0; i < n_points; ++ i) {
            equations[i].print();
        }
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

    virtual void load() {
        bezier2d.preprocess();
        return;
    }

    virtual void print() {
        bezier2d.print();
        return;
    }

    virtual Color_F color(const Vector3D &x) {
        return texture.color;
    }

    inline void plane_case(double y, const Ray &ray, double &ut, Vector3D &gn, bool is_front) {
        double t = (y - ray.o.y) / ray.d.y;
        if(t < 0 || t > ut) return;
        if(bezier2d.in(Vector2D(ray.o.x + ray.d.x * t, ray.o.z + ray.d.z * t)))
           ut = t, gn = is_front ? Vector3D(0, -1, 0) : Vector3D(0, 1, 0);
        return;
    }

    virtual double intersect(const Ray &ray, Vector3D &gn) {
        Vector2D gn2d;
        double t = bezier2d.solve(ray, range_y, gn2d);
        if(t > 0) gn = Vector3D(gn2d.x, 0, gn2d.y);
        if(fabs(ray.d.y) > eps) {
            plane_case(range_y.l, ray, t, gn, true);
            plane_case(range_y.r, ray, t, gn, false);
        }
        return t;
    }
};

# endif
