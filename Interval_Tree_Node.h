#include "AVL_Tree_Node.h"
using namespace std;

class  Interval_Tree_Node {
    private:
    public:
        int q_mid;
        int num_arcs;
        AVL_Tree_Node<Arc_Endpoint>* T_L;
        AVL_Tree_Node<Arc_Endpoint>* T_R;
        Interval_Tree_Node* I_left;
        Interval_Tree_Node* I_right;
        void logical_or(int* a, int* b) {
            for (int z=0; z<3; z++) {
                a[z] = a[z] | b[z];
            }
        }
        Interval_Tree_Node* search(Arc* arc) {
            Interval_Tree_Node* I_mid = this;
            while ((arc -> r < I_mid -> q_mid) || (arc -> l > I_mid -> q_mid)) {
                I_mid = (arc -> r < I_mid -> q_mid) ? I_mid -> I_left : I_mid -> I_right;
            }
            return I_mid;
        }
        Interval_Tree_Node(int q_mid) {
            this -> q_mid = q_mid;
            this -> T_L = NULL;
            this -> T_R = NULL;
            this -> I_left = NULL;
            this -> I_right = NULL;
            this -> num_arcs = 0;
        }
        void add_node(int new_q_mid) {
            Interval_Tree_Node* I_mid = this;
            while ((new_q_mid < I_mid -> q_mid && I_mid -> I_left != NULL) || (new_q_mid > I_mid -> q_mid && I_mid -> I_right != NULL)) {
                I_mid = (new_q_mid < I_mid -> q_mid) ? I_mid -> I_left : I_mid -> I_right;
            }
            if (new_q_mid == I_mid -> q_mid) return;
            if (new_q_mid < I_mid -> q_mid) {
                I_mid -> I_left = new Interval_Tree_Node(new_q_mid);
            } else {
                I_mid -> I_right = new Interval_Tree_Node(new_q_mid);
            }
        }
        void add_Arc(Arc* arc, int shelf, int level) {
            Interval_Tree_Node* match_node = search(arc);
            Arc_Endpoint* left_endpoint = new Arc_Endpoint(arc -> l, shelf, level);
            Arc_Endpoint* right_endpoint = new Arc_Endpoint(arc -> r, shelf, level);
            // cout << "match_node q_mid: " << match_node -> q_mid << " Arc: (" << arc -> l << ", " << arc -> r << ")\n";
            if (match_node -> T_L == NULL) {
                match_node -> T_L = new AVL_Tree_Node<Arc_Endpoint>(left_endpoint);
                match_node -> T_R = new AVL_Tree_Node<Arc_Endpoint>(right_endpoint);
            } else {
                match_node -> T_L = match_node -> T_L -> add_node(left_endpoint);
                match_node -> T_R = match_node -> T_R -> add_node(right_endpoint);
            }
            match_node -> num_arcs++;
        }
        /*void delete_Interval(Arc* arc, int shelf, int level) {
            Interval_Tree_Node* match_node = search(arc);
            Arc_Endpoint left_endpoint = *(new Arc_Endpoint(arc -> l, shelf, level));
            Arc_Endpoint right_endpoint = *(new Arc_Endpoint(arc -> r, shelf, level));
            match_node -> T_L -> delete_node(left_endpoint);
            match_node -> T_R -> delete_node(right_endpoint);
        }*/
        int* query(Arc_Endpoint* arc_ep, int shelf, int* num_intersects) {
            Interval_Tree_Node* I_mid = this;
            int* levels = (int*)calloc(3, sizeof(int));
            int* intersect_levels = (int*)calloc(3, sizeof(int));
            int qk = arc_ep -> val;
            // cout << "(qk: " << qk << ", q_mid: " << I_mid -> q_mid << ")" << endl;
            if (qk == I_mid -> q_mid ) {
                if (I_mid -> T_R != NULL) {
                    intersect_levels = I_mid -> T_R -> endpoints_greater_than_equal(arc_ep, shelf, num_intersects);
                    logical_or(levels, intersect_levels);
                }
            } else {
                if (qk < I_mid -> q_mid) {
                    if (I_mid -> T_L != NULL) {
                        intersect_levels = I_mid -> T_L -> endpoints_less_than_equal(arc_ep, shelf, num_intersects);
                        logical_or(levels, intersect_levels);
                    }
                } else if (qk > I_mid -> q_mid) {
                    if (I_mid -> T_R != NULL) {
                        intersect_levels = I_mid -> T_R -> endpoints_greater_than_equal(arc_ep, shelf, num_intersects);
                        logical_or(levels, intersect_levels); 
                    }
                }
                if (I_mid -> I_left != NULL) 
                    intersect_levels = I_mid -> I_left -> query(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
                if (I_mid -> I_right != NULL) 
                    intersect_levels = I_mid -> I_right -> query(arc_ep, shelf, num_intersects);
                logical_or(levels, intersect_levels);
            }
            // cout << "Returned levels after for (qk: " << qk << ", q_mid: " << I_mid -> q_mid << ")\n";
            // for (int z=0; z<3; z++) {
            //     cout << levels[z] << " ";
            // }
            // cout << "\n\n";
            return levels;
        }
        int* intersect_arcs(Arc* arc, int shelf, int* num_intersects) {
            Interval_Tree_Node* I_mid = this;
            int* levels = (int*)calloc(3, sizeof(int));
            if (arc -> l > I_mid -> q_mid || arc -> r < I_mid -> q_mid) {
                return levels;
            }
            int* intersect_levels = (int*)calloc(3, sizeof(int));
            // cout << "match_node q_mid: " << I_mid -> q_mid << endl;

            Arc_Endpoint* arc_ep = new Arc_Endpoint(arc -> l, shelf, -1);
            if (I_mid -> T_R != NULL) 
                intersect_levels = I_mid -> T_R -> endpoints_greater_than_equal(arc_ep, shelf, num_intersects);
            logical_or(levels, intersect_levels);
            // cout << "Returned levels array after I_mid for (" << arc_ep -> val << ", " << shelf << ")\n";
            // for (int z=0; z<3; z++) {
            //     cout << levels[z] << " ";
            // }
            // cout << "\n\n";

            if (I_mid -> I_left != NULL) 
                intersect_levels = I_mid -> I_left -> intersect_arcs(arc, shelf, num_intersects);
            logical_or(levels, intersect_levels);

            if (I_mid -> I_right != NULL)
                intersect_levels = I_mid -> I_right -> intersect_arcs(arc, shelf, num_intersects);
            logical_or(levels, intersect_levels);

            return levels;
        }
        /*int* intersect_arcs(Arc* arc, int shelf, int* num_intersects) {
            int* levels = (int*)calloc(3, sizeof(int));
            int* intersect_levels = (int*)calloc(3, sizeof(int));
            Interval_Tree_Node* match_node = search(arc);
            cout << "match_node q_mid: " << match_node -> q_mid << endl;

            Arc_Endpoint* arc_ep = new Arc_Endpoint(arc -> l, shelf, -1);
            if (match_node -> T_R != NULL) 
                intersect_levels = match_node -> T_R -> endpoints_greater_than_equal(arc_ep, shelf, num_intersects);
            logical_or(levels, intersect_levels);
            cout << "Returned levels array after I_mid for (" << arc_ep -> val << ", " << shelf << ")\n";
            for (int z=0; z<3; z++) {
                cout << levels[z] << " ";
            }
            cout << "\n\n";

            if (match_node -> I_left != NULL) 
                intersect_levels = match_node -> I_left -> query(arc_ep, shelf, num_intersects);
            logical_or(levels, intersect_levels);
            cout << "Returned levels array after query left for (" << arc_ep -> val << ", " << shelf << ")\n";
            for (int z=0; z<3; z++) {
                cout << levels[z] << " ";
            }
            cout << "\n\n";

            arc_ep = new Arc_Endpoint(arc -> r, shelf, -1);
            if (match_node -> I_right != NULL) 
                intersect_levels = match_node -> I_right -> query(arc_ep, shelf, num_intersects);
            logical_or(levels, intersect_levels);
            cout << "Returned levels array after query right for (" << arc_ep -> val << ", " << shelf << ")\n";
            for (int z=0; z<3; z++) {
                cout << levels[z] << " ";
            }
            cout << "\n\n";

            return levels;
        }*/
};