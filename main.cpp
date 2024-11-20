#include <fstream>
#include <vector>
#include <queue>
#include "Interval_Tree_Node.h"
#define START 0
#define END 360
using namespace std;

Interval_Tree_Node* initialise_interval_tree(int, int);
void compute_or(int[], int[]);
void add_arc(vector<Interval_Tree_Node*>&, Arc*, int, int, int);
void add_reverse_arc(vector<AVL_Tree_Node<Arc>* >&, Arc*, int, int, int);
void print_colors(vector<int>, vector<int>, int);
// void print_preorder_itvl(Interval_Tree_Node*);
// void print_preorder_avl(AVL_Tree_Node<Arc_Endpoint>*);
int main() {
    cout << "Number of arcs: ";
    int n = 0;
    cin >> n;
    vector<Interval_Tree_Node*> prefix_union_arcs = vector<Interval_Tree_Node*>(n, NULL);
    vector<AVL_Tree_Node<Arc>* > prefix_union_reverse_arcs = vector<AVL_Tree_Node<Arc>* >(n, NULL);
    for (int z = 0; z < n; z++) {
        prefix_union_arcs[z] = initialise_interval_tree(START, END);
    }
    // for (int z = 0; z < n; z++) {
    //     print_preorder_itvl(prefix_union_arcs[z]);
    //     cout << endl;
    // }
    // cout << endl;
    vector<int> arc_shelf = vector<int>(n, -1);
    vector<int> arc_level = vector<int>(n, -1);
    Arc* arc = NULL;
    for (int z = 0; z < n; z++) {
        cout << "Arc " << (z+1) << ": ";
        int l, r;
        cin >> l >> r;
        arc = (Arc*)calloc(1, sizeof(Arc));
        arc -> l = arc -> val = l; arc -> r = r;
        int i = -1, num_inersects = 0;
        int occupied_levels[3];
        do {
            i = i + 1;
            num_inersects = 0;
            int* intersect_levels = (int*)calloc(3, sizeof(int));
            occupied_levels[0] = occupied_levels[1] = occupied_levels[2] = 0;
            if (l > r) {
                Arc* temp_arc1 = new Arc(START, r);
                Arc* temp_arc2 = new Arc(l, END - 1);
                intersect_levels = prefix_union_arcs[i]->intersect_arcs(temp_arc1, i, &num_inersects);
                compute_or(occupied_levels, intersect_levels);
                intersect_levels = prefix_union_arcs[i]->intersect_arcs(temp_arc2, i, &num_inersects);
            } else {
                // intersect_levels = prefix_union_arcs[i]->intersect_arcs(arc, i, &num_inersects);
                Interval_Tree_Node* match_node = prefix_union_arcs[i]->search(arc);
                intersect_levels = match_node->intersect_arcs(arc, i, &num_inersects);
            }
            compute_or(occupied_levels, intersect_levels);
            if (prefix_union_reverse_arcs[i] != NULL) {
                Arc* temp_arc = new Arc(0, 0);
                if (l <= r) {
                    temp_arc -> l = arc -> l;
                    temp_arc -> r = arc -> r;
                }
                intersect_levels = prefix_union_reverse_arcs[i]->intersect_arcs(temp_arc, i, &num_inersects);
            }
            compute_or(occupied_levels, intersect_levels);
        } while (num_inersects > i);
        int level = 1;
        for (; occupied_levels[level - 1] == 1; level++);
        arc_shelf[z] = arc -> shelf = i;
        arc_level[z] = arc -> level = level;
        if (l <= r) add_arc(prefix_union_arcs, arc, i, n, level);
        else add_reverse_arc(prefix_union_reverse_arcs, arc, i, n, level);
        // cout << "AVL Trees:\n";
        // for (int y = 0; y < n; y++) {
        //     cout << "(" << z << ", " << y << ")\n";
        //     print_preorder_avl(prefix_union_arcs[y] -> T_L);
        //     cout << endl;
        //     print_preorder_avl(prefix_union_arcs[y] -> T_R);
        //     cout << endl << endl;
        // }
    }
    cout << endl << "Colors: \n";
    print_colors(arc_shelf, arc_level, n);
    return 0;
}
// void print_preorder_itvl(Interval_Tree_Node* root) {
//     if (root == NULL) return;
//     print_preorder_itvl(root -> I_left);
//     cout << root -> q_mid << " ";
//     print_preorder_itvl(root -> I_right);
// }
// void print_preorder_avl(AVL_Tree_Node<Arc_Endpoint>* root) {
//     if (root == NULL) return;
//     print_preorder_avl(root -> left_node);
//     cout << "(" << root -> obj -> val << ", " << root -> obj ->shelf << ", " << root -> obj -> level << ") ";
//     print_preorder_avl(root -> right_node);
// }
Interval_Tree_Node* initialise_interval_tree(int start, int end) {
    int mid = start + (end - start) / 2;
    Interval_Tree_Node* itvl_tree = new Interval_Tree_Node(mid);
    if (start == mid) {
        return itvl_tree;
    }
    queue<int*> q;
    int* root_range = (int*)calloc(2, sizeof(int));
    root_range[0] = start; root_range[1] = end;
    q.push(root_range);
    do {
        int* range = q.front();
        q.pop();
        int l = range[0], r = range[1];
        int mid = l + (r - l) / 2;
        itvl_tree->add_node(mid);
        if (l != mid) {
            int* left_range = (int*)calloc(2, sizeof(int));
            int* right_range = (int*)calloc(2, sizeof(int));
            left_range[0] = l; left_range[1] = mid;
            right_range[0] = mid; right_range[1] = r;
            q.push(left_range);
            q.push(right_range);
        }
    } while (!q.empty());
    return itvl_tree;
}
void compute_or(int a[], int b[]) {
    for (int z=0; z<3; z++) {
        a[z] = a[z] | b[z];
    }
}
void add_arc(vector<Interval_Tree_Node*>& prefix_union_arcs, Arc* arc, int i, int n, int level) {
    for (int z = i; z < n; z++) {
        // cout << "z: "
        prefix_union_arcs[z]->add_Arc(arc, i, level);
    }
}
void add_reverse_arc(vector<AVL_Tree_Node<Arc>* >& prefix_union_reverse_arcs, Arc* arc, int i, int n, int level) {
    for (int z = i; z < n; z++) {
        if (prefix_union_reverse_arcs[z] != NULL) prefix_union_reverse_arcs[z]->add_node(arc);
        else prefix_union_reverse_arcs[z] = new AVL_Tree_Node<Arc>(arc);
    }
}
void print_colors(vector<int> arc_shelf, vector<int> arc_level, int n) {
    ofstream fout;
    fout.open("input.txt", ios::app);
    fout << endl;
    for (int z = 0; z < n; z++) {
        cout << "Arc " << (z+1) << ": " << "(" << arc_shelf[z] << ", " << arc_level[z] << ")\n";
        fout << arc_shelf[z] << " " << arc_level[z] << endl;
    }
}

// (0, 5), (0, 2), (2, 5), (0-, 1), (1-, 2), (2-, 3), (3-, 5), (3-, 4), (4-, 5)
//  2-      1-      3-      0-       1-       2-       3-       3-       4-
// (0, 6), (0, 3), (3, 6), (0, 1), (1, 3), (3, 4), (4, 6), (1, 2), (2, 3), (4, 5), (5, 6)
//  3-      1-      4-      0-      1-      3-      5-      1-      2-      4-      5-

/*
10
34 68
146 292
146 211
65 130
3 55
52 104
52 104
3 6
3 167
174 348
*/