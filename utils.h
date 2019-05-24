# include <cmath>
# include <cstdlib>

# define eps 1e-4
# define inf 1e30

inline double rand01() {
    return double(rand()) / RAND_MAX;
}

inline double clamp(double x) {
    return x < 0 ? 0 : (x > 1 ? 1 : x);
}
inline int gray2int(double x) {
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}