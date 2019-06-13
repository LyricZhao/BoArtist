# ifndef __SPPM_H__
# define __SPPM_H__

# include <vector>

# include "objects/api.h"
# include "utils.h"

# define KD_L_CHILD(node) (nodes[(node)].child[0])
# define KD_R_CHILD(node) (nodes[(node)].child[1])
# define KD_POS(node) (nodes[(node)].point.pos)
# define KD_R(node) (nodes[(node)].point.r)
# define KD_INDEX(node) (nodes[(node)].point.index)
# define KD_COLOR(node) (nodes[(node)].point.color)
# define KD_RANGE(node) (nodes[(node)].range)
# define KD_PROB(node) (nodes[(node)].point.prob)

class VisiblePoint {
public:
    int index; Vector3D pos, color, nl; double r, prob;
    VisiblePoint() {}
    VisiblePoint(int _index, Vector3D _pos, Vector3D _color, Vector3D _nl, double _r, double _prob):
        index(_index), pos(_pos), color(_color), nl(_nl), r(_r), prob(_prob) { }
};

class KDNode {
public:
    int child[2];
    VisiblePoint point;
    Range3D range;

    KDNode();
};

class KDTree {
public:
    int root, total_points;
    KDNode *nodes;

    KDTree();
    ~KDTree();

    void t_update(int node, int child);
    void t_build(int &node, int l, int r, int dim);
    void t_query(int node, const Vector3D &pos, const Vector3D &nl, const Color_F &color, Pixel *buffer);

    void free();
    void build(const std:: vector<VisiblePoint> *visiable_points, int n_threads);
    void query(const Vector3D &pos, const Vector3D &nl, const Color_F &color, Pixel *buffer);
};

# endif