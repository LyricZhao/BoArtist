# ifndef __MESH_H__
# define __MESH_H__

# include <algorithm>
# include <cassert>
# include <string>
# include <fstream>
# include <tuple>
# include <vector>

# include "base.h"
# include "object.h"

# define KD_L_CHILD(node) (nodes[(node)].child[0])
# define KD_R_CHILD(node) (nodes[(node)].child[1])
# define KD_RANGE(node) (nodes[(node)].range)
# define KD_SURFACE(node) (nodes[(node)].surface)

namespace obj_ds {
class Surface {
public:
    Vector3D points[3], g, norm;

    inline void calc() {
        g = points[0];
        norm = (points[2] - points[0]).cross(points[1] - points[0]).norm();
        return;
    }

    inline void to_update(const Ray &ray, Vector3D &gn, double &t) {
        Vector3D e1 = points[1] - points[0], e2 = points[2] - points[0], p = ray.d.cross(e2), vt, q;
        double u, v, ct, det = e1.dot(p);
        if(det > 0) vt = ray.o - points[0];
        else vt = points[0] - ray.o, det = -det;
        if(det < eps) return;
        u = vt.dot(p); if(u < 0 || u > det) return;
        q = vt.cross(e1);
        v = ray.d.dot(q); if(v < 0 || u + v > det) return;
        ct = e2.dot(q) / det;
        if(ct > 0 && ct < t)
            t = ct, gn = norm;
        return;
    }
};

class KDNode {
public:
    int child[2];
    Surface surface;
    Range3D range;

    KDNode() { }
};

class KDTree {
public:
    int root, total_surfaces;
    KDNode *nodes;

    KDTree() {
        root = 0;
        nodes = nullptr;
        return;
    }

    ~KDTree() {
        KDTree:: free();
        return;
    }

    void t_update(int node, int child) {
        if(child == -1) return;
        KD_RANGE(node).expand(KD_RANGE(child));
        return;
    }

    void t_build(int &node, int l, int r, int dim) {
        if(l >= r) return;
        int mid = (l + r) >> 1;
        node = mid;
        std:: nth_element(nodes + l, nodes + mid, nodes + r,
            [dim](const KDNode &a, const KDNode &b) {
                return a.surface.g.dim(dim) < b.surface.g.dim(dim);
            });
        KD_RANGE(node).surface(KD_SURFACE(node).points);
        t_build(KD_L_CHILD(node), l, mid, (dim + 1) % 3);
        t_build(KD_R_CHILD(node), mid + 1, r, (dim + 1) % 3);
        t_update(node, KD_L_CHILD(node)), t_update(node, KD_R_CHILD(node));
        return;
    }

    double t_query(int node, const Ray &ray, Vector3D &gn, double &t) {
        KD_SURFACE(node).to_update(ray, gn, t);
        double est[2] = {inf, inf};
        if(KD_L_CHILD(node) != -1) est[0] = KD_RANGE(KD_L_CHILD(node)).estimate(ray);
        if(KD_R_CHILD(node) != -1) est[1] = KD_RANGE(KD_R_CHILD(node)).estimate(ray);
        int choice = est[0] < est[1] ? 0 : 1;
        for(int i = 0; i < 2; ++ i, choice ^= 1) if(est[choice] < t)
            t_query(nodes[node].child[choice], ray, gn, t);
        return t;
    }

    void free() {
        if(nodes != nullptr) {
            std:: free(nodes);
            nodes = nullptr;
        }
        return;
    }

    # define BUFFER_MAX_LENGTH 256
    void build(std:: string path, const Vector3D &shift, double scale) {
        assert(path != "");

        std:: vector<Vector3D> points;
        std:: vector<std:: tuple<int, int, int> > tuples;
        char buffer[BUFFER_MAX_LENGTH];
        std:: ifstream input(path);
        while(input.getline(buffer, BUFFER_MAX_LENGTH)) {
            std:: string line = buffer;
            if(!line.length()) continue;
            if(line[0] == 'v' && line[1] == ' ') {
                Vector3D v;
                for(int i = 0, pos = 0; i < 3; ++ i)
                    v.dim_addr(i) = atof(read_number(line, pos).c_str());
                points.push_back(v * scale + shift);
            }
            if(line[0] == 'f') {
                int v[3];
                bool read_twice = (line.find('/') != line.npos);
                for(int i = 0, pos = 0; i < 3; ++ i) {
                    v[i] = atoi(read_number(line, pos).c_str()) - 1;
                    if(read_twice) read_number(line, pos);
                }
                tuples.push_back(std:: make_tuple(v[0], v[1], v[2]));
            }
        }

        total_surfaces = tuples.size();
        nodes = (KDNode *)std:: malloc(sizeof(KDNode) * total_surfaces);
        for(int i = 0; i < total_surfaces; ++ i) {
            KD_SURFACE(i).points[0] = points[std:: get<0>(tuples[i])];
            KD_SURFACE(i).points[1] = points[std:: get<1>(tuples[i])];
            KD_SURFACE(i).points[2] = points[std:: get<2>(tuples[i])];
            KD_SURFACE(i).calc();
            KD_L_CHILD(i) = KD_R_CHILD(i) = -1;
        }
        t_build(root, 0, total_surfaces, 0);
        return;
    }
    # undef BUFFER_MAX_LENGTH

    double query(const Ray &ray, Vector3D &gn) {
        double t = inf;
        t_query(root, ray, gn, t);
        return t < inf ? t : 0;
    }
};
}

/* Texture is contained */
class Mesh: public Object {
public:
    Vector3D shift; std:: string path;
    obj_ds:: KDTree *tree; double scale;

    Mesh(Vector3D _shift, std:: string _path, ReflectType _reflect, double _ior, double _scale, Texture _texture):
        shift(_shift), path(_path), scale(_scale), Object(_reflect, _ior, _texture, Color_F()) { }

    virtual void debug() {
        return;
    }

    virtual void load() {
        tree = new obj_ds:: KDTree();
        tree -> build(path, shift, scale);
        return;
    }

    virtual Color_F color(const Vector3D &x) {
        return texture.color;
    }

    virtual double intersect(const Ray &ray, Vector3D &gn) {
        double t = tree -> query(ray, gn);
        return t;
    }
};

# endif