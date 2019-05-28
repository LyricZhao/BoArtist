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

# endif