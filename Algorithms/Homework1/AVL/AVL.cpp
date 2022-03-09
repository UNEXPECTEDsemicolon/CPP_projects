#include <algorithm>

template <typename T>
class AVL {
private:
    size_t size = 0;

    struct Node {
        T val = 0;
        int depth = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
        Node(T val = 0): val(val) {}

        Node(const Node& orig): val(orog.val), left(orig.left), right(orig.right) {}

        ~Node() {
            if (left)
                delete left;
            if (right)
                delete right;
        }

        Node* find(T x) {
            if (this == nullptr)
                return this;
            if (x == val)
                return this;
            else if (x < val)
                return left->find(x);
            else
                return right->find(x);
        }

        void normalize() {
            int delta = (left ? left->depth : 0) - (right ? right->depth : 0);
            if (delta <= -2) {
                right->parent = parent;
                parent = right;
                right = parent->left;
                parent->left = this;
                parent->parent->normal();
            } else if (delta >= 2) {
                left->parent = parent;
                parent = left;
                left = parent->right;
                parent->right = this;
                parent->parent->normal();
            }
        }
    };

    Node* root = nullptr;

public:
    AVL() {}

    ~AVL() {
        if (root)
            delete root;
    }

    bool find(T x) {
        return root->find(x) != nullptr;
    }

    void insert(T x) {
        auto &target = root->find(x);
        if (target != nullptr)
            return;
        target = new Node(x);
        target->normalize();
    }

    void del(T x) {
        auto &target = root->find(x);
        if (target == nullptr)
            return;
        if (target->left == nullptr) {
            target->right->parent = target->parent;
            (this == )
            target->parent->
        }
    }
};