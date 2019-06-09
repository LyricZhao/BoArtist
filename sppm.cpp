# include <algorithm>
# include <cassert>

# include "sppm.h"

KDNode:: KDNode() {
    child[0] = child[1] = -1;
    return;
}

KDTree:: KDTree() {
    root = 0;
    nodes = nullptr;
    return;
}

KDTree:: ~KDTree() {
    KDTree:: free();
    return;
}

void KDTree:: free() {
    if(nodes != nullptr) {
        std:: free(nodes);
        nodes = nullptr;
    }
    return;
}

void KDTree:: t_update(int node, int child) {
    if(child == -1) return;
    nodes[node].range.expand(nodes[child].range);
    return;
}

void KDTree:: t_build(int &node, int l, int r, int dim) {
    if(l >= r) return;
    int mid = (l + r) >> 1;
    node = mid;
    nodes[node].range.overstate(nodes[node].point.pos, nodes[node].point.r);
    std:: nth_element(nodes + l, nodes + mid, nodes + r,
        [dim](const KDNode &a, const KDNode &b) {
            return a.point.pos.dim(dim) < b.point.pos.dim(dim);
        });
    t_build(KD_L_CHILD(node), l, mid, (dim + 1) % 3);
    t_build(KD_R_CHILD(node), mid + 1, r, (dim + 1) % 3);
    t_update(node, KD_L_CHILD(node)), t_update(node, KD_R_CHILD(node));
    return;
}

void KDTree:: build(const std:: vector<VisiblePoint> &visible_points) {
    assert(nodes == nullptr);
    int total_points = visible_points.size();
    nodes = (KDNode*) std:: malloc(sizeof(KDNode) * total_points);
    for(int i = 0; i < total_points; ++ i)
        nodes[i].point = visible_points[i];
    t_build(root, 0, total_points, 0);
    return;
}