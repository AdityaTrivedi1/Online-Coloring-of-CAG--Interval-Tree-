class Arc {
    private:
    public:
        int l;
        int r;
        int val;
        int shelf;
        int level;
        Arc(int l, int r) {
            this->l = l;
            this->r = r;
            this->val = this->l;
            this->shelf = -1;
            this->level = -1;
        }
};

class Arc_Endpoint {
    private:
    public:
        int val;
        int shelf;
        int level;
        Arc_Endpoint(int val, int shelf, int level) {
            this->val = val;
            this->shelf = shelf;
            this->level = level;
        }
};