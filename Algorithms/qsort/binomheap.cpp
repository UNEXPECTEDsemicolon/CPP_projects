#include <stdio.h>
#include <iostream>
#include <utility>

template <typename T>
class BinomHeap {
public:
    struct Node {
        T val = -1;
        size_t size = 0;
        Node *parent = nullptr;
        Node *leftchild = nullptr;
        Node *right = nullptr;
        Node() {}
        Node(const T &val): val(val), size((bool)val) {}
        Node(const Node* node): val(node->val), size(node->size) {
            if (node->leftchild != nullptr)
                leftchild = new Node(node->leftchild);
            if (node->right != nullptr)
                right = new Node(node->right);
        }
        ~Node() {
            if (leftchild != nullptr) delete leftchild;
            leftchild = nullptr;
            if (right != nullptr) delete right;
            right = nullptr;
        }
        void decreaseKey(T k) {
            val = k;
            if (parent != nullptr && val < parent->val) {
                T parents_val = parent->val;
                parent->val = val;
                val = parents_val;
                parent->decreaseKey(val);
            }
        }
        void print(size_t intend = 0) const {
            if (right != nullptr) {
//                printf("right: ");
                right->print(intend);
            }
            for (size_t i = 0; i < intend; ++i) {
                printf("    ");
            }
            printf("%lld\n", val);
            if (leftchild != nullptr) {
//                printf("leftchild: ");
                leftchild->print(intend + 1);
            }
        }
    };
    struct RootNode {
        RootNode* left = nullptr;
        RootNode* right = nullptr;
        Node *tree;
//        RootNode(): tree(new Node()) {}
        RootNode(const Node *node = nullptr): tree(node != nullptr ? new Node(node) : new Node()) {}

        ~RootNode() {}
        void hardDelete() {
            delete tree;
            delete this;
        }
        RootNode *next() const {
            return right;
        }
        RootNode *prev() const {
            return left;
        }
//        RootNode *operator+ (int x) {
//            return right;
//        }
        RootNode *operator- (int x) {
            return left;
        }
        void print() const {
            tree->print();
        }
    };
    struct List {
        RootNode *begin = nullptr;
        RootNode *end = nullptr;
//        List() {
//            begin = new RootNode();
//            end = new RootNode();
//        }
        List(const Node *node = nullptr) {
            begin = new RootNode(node);
            end = new RootNode();
            begin->right = end;
            end->left = begin;
        }
        void destroy() {
            for (RootNode *it = begin->right; it != end; ) {
                it = it->right;
                it->left->hardDelete();
            }
            begin->tree->val = -1;
            begin->right = end;
            end->left = begin;
        }
        ~List() {
            destroy();
            delete begin;
            delete end;
        }
        List &operator= (const List &right_list) {
            destroy();
            for (auto it = right_list.begin; it != right_list.end; it = it->next()) {
                insert(it->tree, end);
            }
            return *this;
        }
        Node *insert(const Node *node, RootNode *dest) {
            if (begin->tree->val != -1) {
                if (dest->left != nullptr) {
                    dest->left->right = new RootNode(node);
                    dest->left->right->left = dest->left;
                    dest->left = dest->left->right;
                    dest->left->right = dest;
                }
                else {
                    dest->left = new RootNode(node);
                    dest->left->right = dest;
                    begin = dest->left;
                }
            }
            else {
                delete begin->tree;
                begin->tree = new Node(node);
                begin->right = end;
                end->left = begin;
            }
            return end->left->tree;
        }
        void erase(RootNode *node, bool safe = false) {
            if (node != begin) node->left->right = node->right;
            if (node != end) node->right->left = node->left;
            if (!safe) node->hardDelete();
            else delete node;
        }
        void merge(const List &right_list) {
            auto it1 = begin, it2 = right_list.begin;
            for (; it1 != end; it1 = it1->next())
                for (; it2 != right_list.end && it2->tree->size < it1->tree->size; it2 = it2->next())
                    insert(new Node(it2->tree), it1);
            for (; it2 != right_list.end; it2 = it2->next())
                insert(new Node(it2->tree), it1);
        }
    };
    List root_list;
public:
    BinomHeap() {}
    BinomHeap(Node *root): root_list(root) {}
    BinomHeap(T x) {
        auto *tmp = new Node(x);
        root_list = List(tmp);
        delete tmp;
    }
    BinomHeap<T> &operator= (const BinomHeap<T> &orig) {
        root_list = orig.root_list;
        return *this;
    }
    ~BinomHeap() {}
    void merge(const BinomHeap<T> &h1, Node *&result) {
        root_list.merge(h1.root_list);
        result = root_list.end->left->tree;
        for (auto it = root_list.begin->next();
             it != root_list.end; it = it->next()) {
            if ((it->prev())->tree->size == it->tree->size && (it == root_list.end->prev() ||
                it->tree->size != it->next()->tree->size)) {
                if (it->tree->val <= it->prev()->tree->val) {
                    it->prev()->tree->right = it->tree->leftchild;
                    it->tree->leftchild = it->prev()->tree;
                    it->tree->leftchild->parent = it->tree;
                    root_list.erase(it->prev(), true);
                    ++(it->tree->size);
                } else {
                    it->tree->right = it->prev()->tree->leftchild;
                    it->prev()->tree->leftchild = it->tree;
                    it->prev()->tree->leftchild->parent = it->prev()->tree;
                    ++(it->prev()->tree->size);
                    it = it->prev();
                    root_list.erase(it->next(), true);
                }
            }
        }
    }
    void insert(T x, Node *&result) {
        merge(x, result);
    }
    RootNode *getMin() const {
        T min = root_list.begin->tree->val;
        RootNode *best = root_list.begin;
        for (auto it = root_list.begin;
             it != root_list.end; it = it->next()) {
            if (it->tree->val < best->tree->val)
                best = it;
        }
        return best;
    }
    void extractMin() {
        auto tmp = getMin();
        RootNode *minroot = new RootNode(*tmp);
        root_list.erase(tmp);
        BinomHeap<T> new_h;
        for (auto it = minroot->tree->leftchild; it != nullptr; it = it->right) {
            new_h.root_list.insert(it, new_h.root_list.end);
        }
        Node *trash;
        merge(new_h, trash);
        delete minroot;
    }
    void deleteKey(Node *node) {
        node->decreaseKey(-1);
        extractMin();
    }
    void print() {
        printf("[\n");
        for (auto it = root_list.begin; it != root_list.end; it = it->next()) {
//            printf("iter\n");
            it->print();
        }
        printf("]\n");
    }
};

int main() {
//    BinomHeap<int> h(5);
//    h.print();
//    std::cout << "after print\n";
//
//    std::cout << h.getMin() << '\n';
//    h.insert(6);
//    h.print();
//    h.insert(3);
//    h.print();
//    std::cout << h.getMin() << '\n';

    size_t n, m;
    scanf("%zu %zu", &n, &m);
    auto *heaps = new BinomHeap<long long> [n];
//    for (size_t i = 0; i < n; ++i) heaps[i] = BinomHeap<long long>();
    auto *ind = new std::pair<BinomHeap<long long>::Node*, BinomHeap<long long>*> [m];
    for (size_t q = 0, adds = 0; q < m; ++q) {
        int act;
        scanf("%d", &act);
        if (act == 0) {
            size_t a;
            long long v;
            scanf("%zu %lld", &a, &v);
            heaps[a-1].insert(v, ind[adds].first);
//            printf("!!!!!%lld\n", ind[adds].first->val);
            ind[adds].second = &heaps[a-1];
            ++adds;
        }
        else if (act == 1) {
            size_t a, b;
            scanf("%zu %zu", &a, &b);
            BinomHeap<long long>::Node *trash;
            heaps[b-1].merge(heaps[a-1], trash);
            heaps[a-1] = BinomHeap<long long>();
        }
        else if (act == 2) {
            size_t i;
            scanf("%zu", &i);
            ind[i-1].second->deleteKey(ind[i-1].first);
        }
        else if (act == 3) {
            size_t i;
            long long v;
            scanf("%zu %lld", &i, &v);
            ind[i-1].first->decreaseKey(v);
        }
        else if (act == 4) {
            size_t a;
            scanf("%zu", &a);
            printf("%lld\n", heaps[a-1].getMin()->tree->val);
        }
        else if (act == 5) {
            size_t a;
            scanf("%zu", &a);
            heaps[a-1].extractMin();
        }
        for (size_t i = 0; i < n; ++i) {
            printf("heap#%zu:\n", i);
            heaps[i].print();
        }
    }
    delete [] heaps;
    delete [] ind;
}
