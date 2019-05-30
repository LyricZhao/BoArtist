# ifndef __UTILS_H__
# define __UTILS_H__

# include <cmath>
# include <cstdlib>

# define eps 1e-8
# define inf 1e20

inline double rand01() {
    return double(rand()) / RAND_MAX;
}

inline double clamp(double x) {
    return x < 0 ? 0 : (x > 1 ? 1 : x);
}

inline int gray2int(double x) {
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

inline void update_solve(double &x, double z) {
    if(z > 0 && z < x) x = z;
    return;
}

inline void s_upd_min(double &t_min, double v) {
    if(v > eps && v < t_min) t_min = v;
    return;
}

inline void t_upd_min(double &t_min, double v) {
    if(v > eps && v < inf && v > t_min) t_min = v;
    return;
}

inline void upd_min(double &t_min, double v) {
    if(v < t_min) t_min = v;
    return;
}

inline void upd_max(double &t_max, double v) {
    if(v > t_max) t_max = v;
    return;
}

# endif