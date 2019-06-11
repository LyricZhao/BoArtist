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
    KD_RANGE(node).expand(KD_RANGE(child));
    return;
}

void KDTree:: t_build(int &node, int l, int r, int dim) {
    if(l >= r) return;
    int mid = (l + r) >> 1;
    node = mid;
    KD_RANGE(node).overstate(KD_POS(node), KD_R(node));
    std:: nth_element(nodes + l, nodes + mid, nodes + r,
        [dim](const KDNode &a, const KDNode &b) {
            return a.point.pos.dim(dim) < b.point.pos.dim(dim);
        });
    t_build(KD_L_CHILD(node), l, mid, (dim + 1) % 3);
    t_build(KD_R_CHILD(node), mid + 1, r, (dim + 1) % 3);
    t_update(node, KD_L_CHILD(node)), t_update(node, KD_R_CHILD(node));
    return;
}

void KDTree:: build(const std:: vector<VisiblePoint> *visible_points, int n_threads) {
    assert(nodes == nullptr);
    int total_points = 0;
    for(int i = 0; i < n_threads; ++ i) total_points += visible_points[i].size();
    nodes = (KDNode*) std:: malloc(sizeof(KDNode) * total_points);
    for(int t = 0, i = 0; t < n_threads; ++ t)
        for(int j = 0; j < visible_points[t].size(); ++ j, ++ i)
            nodes[i] = KDNode(), nodes[i].point = visible_points[t][j];
        
    t_build(root, 0, total_points, 0);
    return;
}

void KDTree:: t_query(int node, const Vector3D &pos, const Vector3D &nl, const Color_F &color, Pixel *buffer) {
    /* TODO: consider prob */
    if((nodes[node].point.pos - pos).length2() <= sqr(nodes[node].point.r) && nodes[node].point.nl.dot(nl) >= 0)
        buffer[KD_INDEX(node)].add(KD_COLOR(node).mul(color));
        
    if(KD_L_CHILD(node) != -1 && KD_RANGE(KD_L_CHILD(node)).contain(pos))
        t_query(KD_L_CHILD(node), pos, nl, color, buffer);
    if(KD_R_CHILD(node) != -1 && KD_RANGE(KD_R_CHILD(node)).contain(pos))
        t_query(KD_R_CHILD(node), pos, nl, color, buffer);    
    return;
}

void KDTree:: query(const Vector3D &pos, const Vector3D &nl, const Color_F &color, Pixel *buffer) {
    t_query(root, pos, nl, color, buffer);
    return;
}