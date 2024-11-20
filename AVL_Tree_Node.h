#include <iostream>
#include <cstdlib>
#include "AVL_Tree_Node_Templates.h"
using namespace std;

template <typename T> class AVL_Tree_Node {
    private:
        void print_preorder_avl(AVL_Tree_Node<Arc_Endpoint>* root) {
            if (root == NULL) return;
            print_preorder_avl(root -> left_node);
            cout << "(" << root -> obj -> val << ", " << root -> obj ->shelf << ", " << root -> obj -> level << ") ";
            print_preorder_avl(root -> right_node);
        }
        int max(int a, int b) {
            return (a >= b) ? a : b;
        }
        void logical_or(int a[], int b[]) {
            for (int z=0; z<3; z++) {
                a[z] = a[z] | b[z];
            }
        }
        int left_subtree_height() {
            return (this -> left_node != NULL) ? this -> left_node -> height : -1;
        }
        int right_subtree_height() {
            return (this -> right_node != NULL) ? this -> right_node -> height : -1;
        }
        int num_nodes_left_subtree() {
            return (this -> left_node != NULL) ? this -> left_node -> num_nodes_subtree : 0;
        }
        int num_nodes_right_subtree() {
            return (this -> right_node != NULL) ? this -> right_node -> num_nodes_subtree : 0;
        }
        int get_balance_factor() {
            return this -> left_subtree_height() - this -> right_subtree_height();
        }
        AVL_Tree_Node* left_rotate() {
            AVL_Tree_Node* curr_node = this;
            AVL_Tree_Node* right_node = this -> right_node;
            curr_node -> right_node = right_node -> left_node;
            right_node -> left_node = curr_node;
            curr_node -> height = max(curr_node -> left_subtree_height(), curr_node -> right_subtree_height()) + 1;
            right_node -> height = max(right_node -> left_subtree_height(), right_node -> right_subtree_height()) + 1;
            return right_node;
        }
        AVL_Tree_Node* right_rotate() {
            AVL_Tree_Node* curr_node = this;
            AVL_Tree_Node* left_node = curr_node -> left_node;
            curr_node -> left_node = left_node -> right_node;
            left_node -> right_node = curr_node;
            curr_node -> height = max(curr_node -> left_subtree_height(), curr_node -> right_subtree_height()) + 1;
            left_node -> height = max(left_node -> left_subtree_height(), left_node -> right_subtree_height()) + 1;
            return left_node;
        }
        AVL_Tree_Node* balance_node() {
            AVL_Tree_Node* curr_node = this;
            int bf = curr_node -> get_balance_factor();
            if (bf == 2) {
                int bf_left = curr_node -> left_node -> get_balance_factor();
                if (bf_left == 1) {
                    curr_node = curr_node -> right_rotate();
                } else if (bf_left == -1) {
                    curr_node -> left_node = curr_node -> left_node -> left_rotate();
                    curr_node = curr_node -> right_rotate();
                }
            } else if (bf == -2) {
                int bf_right = curr_node -> right_node -> get_balance_factor();
                if (bf_right == -1) {
                    curr_node = curr_node -> left_rotate();
                } else if (bf_right == 1) {
                    curr_node -> right_node = curr_node -> right_node -> right_rotate();
                    curr_node = curr_node -> left_rotate();
                }
            }
            return curr_node;
        }
    public:
        T* obj;
        AVL_Tree_Node* left_node;
        AVL_Tree_Node* right_node;
        int height;
        int num_nodes_subtree;
        AVL_Tree_Node<T>(T* obj) {
            this -> obj = obj;
            this -> height = 0;
            this -> num_nodes_subtree = 1;
        }
        AVL_Tree_Node* add_node(T* obj) {
            AVL_Tree_Node* curr_node = this;
            if (obj -> val < curr_node -> obj -> val) {
                if (curr_node -> left_node == NULL) {
                    curr_node ->left_node = new AVL_Tree_Node<T>(obj);
                }
                else {
                    curr_node -> left_node = curr_node -> left_node -> add_node(obj);
                }
            } else if (obj -> val >= curr_node -> obj -> val) {
                if (curr_node -> right_node == NULL) {
                    curr_node -> right_node = new AVL_Tree_Node<T>(obj);
                }
                else {
                    curr_node -> right_node = curr_node -> right_node -> add_node(obj);
                }
            }
            curr_node = curr_node -> balance_node();
            curr_node -> height = max(curr_node -> left_subtree_height(), curr_node -> right_subtree_height()) + 1;
            curr_node -> num_nodes_subtree = curr_node -> num_nodes_left_subtree() + curr_node -> num_nodes_right_subtree() + 1;
            return curr_node;
        }
        /*AVL_Tree_Node* delete_node(T* obj) {
            AVL_Tree_Node* curr_node = this;
            if (curr_node == NULL) return curr_node;
            if (obj -> val < curr_node -> obj -> val) {
                curr_node -> left_node = curr_node -> left_node -> delete_node(obj);
            } else if (obj -> val > curr_node -> obj -> val || (obj -> val == curr_node -> obj -> val && (obj -> shelf != curr_node -> obj -> shelf && obj -> level != curr_node -> obj -> level))) {
                curr_node -> right_node = curr_node -> right_node -> delete_node(obj);
                curr_node = curr_node -> balance_node();
            } else {
                AVL_Tree_Node* temp = NULL;
                if (curr_node -> left_node != NULL && curr_node -> right_node != NULL) {
                    temp = curr_node -> right_node;
                    while (temp -> left_node != NULL) temp = temp -> left_node;
                    curr_node -> obj = temp -> obj;
                    curr_node -> right_node = curr_node -> right_node -> delete_node(temp -> obj);
                    return curr_node;
                }
                temp = (curr_node -> left_node != NULL) ? curr_node -> left_node : curr_node -> right_node;
                free(curr_node);
                curr_node = NULL;
                return temp;
            }
            curr_node = curr_node -> balance_node();
            curr_node -> height = max(curr_node -> left_subtree_height(), curr_node -> right_subtree_height()) + 1;
            curr_node -> num_nodes_subtree = curr_node -> num_nodes_left_subtree() + curr_node -> num_nodes_right_subtree() + 1;
            return curr_node;
        }*/
        int* endpoints_less_than_equal(T* arc_ep, int shelf, int* num_intersects) {
            AVL_Tree_Node* curr_node = this;
            // cout << "LTE AVL Tree:\n";
            // print_preorder_avl(curr_node);
            int qk = arc_ep -> val;
            int* levels = (int*)calloc(3, sizeof(int));
            int* intersect_levels = (int*)calloc(3, sizeof(int));
            if (qk >= curr_node -> obj -> val) {
                (*num_intersects)++;
                if (shelf == curr_node -> obj -> shelf) {
                    int level = curr_node -> obj -> level;
                    levels[level-1] = 1;
                }
                if (curr_node -> left_node != NULL) 
                    intersect_levels = curr_node -> left_node -> endpoints_less_than_equal(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
                if (curr_node -> right_node != NULL) 
                    intersect_levels = curr_node -> right_node -> endpoints_less_than_equal(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
            } else if (qk < curr_node -> obj -> val) {
                if (curr_node -> right_node != NULL) 
                    intersect_levels = curr_node -> right_node -> endpoints_less_than_equal(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
            }
            // cout << "Returned levels array lte for (" << arc_ep -> val << ", " << shelf << ")\n";
            // for (int z=0; z<3; z++) {
            //     cout << levels[z] << " ";
            // }
            // cout << "\n\n";
            return levels;
        }
        int* endpoints_greater_than_equal(T* arc_ep, int shelf, int* num_intersects) {
            AVL_Tree_Node* curr_node = this;
            // cout << "GTE AVL Tree:\n";
            // print_preorder_avl(curr_node);
            int qk = arc_ep -> val;
            int* levels = (int*)calloc(3, sizeof(int));
            int* intersect_levels = (int*)calloc(3, sizeof(int));
            if (qk <= (curr_node -> obj -> val)) {
                (*num_intersects)++;
                if (shelf == curr_node -> obj -> shelf) {
                    int level = curr_node -> obj -> level;
                    levels[level-1] = 1;
                }
                if (curr_node -> right_node != NULL) 
                    intersect_levels = curr_node -> right_node -> endpoints_greater_than_equal(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
                if (curr_node -> left_node != NULL) 
                    intersect_levels = curr_node -> left_node -> endpoints_greater_than_equal(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
            } else if (qk > curr_node -> obj -> val) {
                if (curr_node -> right_node != NULL) 
                    intersect_levels = curr_node -> right_node -> endpoints_greater_than_equal(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
            }
            // cout << "Returned levels array gte for (" << arc_ep -> val << ", " << shelf << ")\n";
            // for (int z=0; z<3; z++) {
            //     cout << levels[z] << " ";
            // }
            // cout << "\n\n";
            return levels;
        }
        int* intersect_arcs(Arc* arc, int shelf, int* num_intersects) {
            AVL_Tree_Node* curr_node = this;
            int* levels = (int*)calloc(3, sizeof(int));
            int* intersect_levels = (int*)calloc(3, sizeof(int));
            if (arc -> r >= curr_node -> obj -> l || arc -> l <= curr_node -> obj -> r) {
                (*num_intersects)++;
                if (shelf == curr_node -> obj -> shelf) {
                    int level = curr_node -> obj -> level;
                    levels[level-1] = 1;
                }
            }
            if (curr_node -> left_node != NULL) 
                intersect_levels = curr_node -> left_node -> intersect_arcs(arc, shelf, num_intersects);
            logical_or(levels, intersect_levels);
            if (curr_node -> right_node != NULL) 
                intersect_levels = curr_node -> right_node -> intersect_arcs(arc, shelf, num_intersects);
            logical_or(levels, intersect_levels);
            return levels;
        }
};
