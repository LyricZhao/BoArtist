# ifndef __SPPM_H__
# define __SPPM_H__

# include <vector>

# include "objects/api.h"
# include "util.h"

# define KD_L_CHILD(node) (nodes[(node)].child[0])
# define KD_R_CHILD(node) (nodes[(node)].child[1])

class VisiblePoint {
public:
    int index; Vector3D pos, color, nl; double r, prob;
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
    int root;
    KDNode *nodes;

    KDTree();
    ~KDTree();

    void t_update(int node, int child);
    void t_build(int &node, int l, int r, int dim);
    void t_query(int node, const VisiblePoint &point);

    void free();
    void build(const std:: vector<VisiblePoint> &visiable_points);
    void query(const VisiblePoint &point);
};

# endif