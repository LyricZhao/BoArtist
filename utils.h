# ifndef __UTILS_H__
# define __UTILS_H__

# include <cmath>
# include <cstdlib>

# define eps 1e-12
# define inf 1e20

inline double sqr(double x) {
    return x * x;
}

inline double rand01() {
    return double(rand()) / RAND_MAX;
}

inline double sqrt_3(double x) {
    return x < 0 ? -pow(-x, 1. / 3) : pow(x, 1. / 3);
}

inline double clamp(double x) {
    return x < 0 ? 0 : (x > 1 ? 1 : x);
}

inline int gray2int(double x) {
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
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

std:: string read_number(std:: string line, int &pos) {
    auto is_number = [] (char c) -> bool {
        return ('0' <= c && c <= '9') || c == '.' || c == '-' || c == 'e';
    };
    int start = pos;
    while(start < line.length() && !is_number(line[start])) ++ start;
    pos = start + 1;
    while(pos < line.length() && is_number(line[pos])) ++ pos;
    return line.substr(start, pos - start);
}

# endif