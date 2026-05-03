/*
 * Задача № 4
 *
 * 4_2. Порядковые статистики на АВЛ-дереве. N команд; в строке A и k:
 * A > 0 — добавить A, A < 0 — удалить |A|.
 * После операции вывести k-ю порядковую статистику.
 * Требование: передача функции сравнения снаружи.
 *
 * Скорость работы - O(log n) на операцию (амортизированно).
 * Потребляемая память - O(n).
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>

template <typename T, typename Compare = std::less<T>>
class AvlOrderStatisticsTree {
public:
    explicit AvlOrderStatisticsTree(Compare comp = Compare{}) : root_(nullptr), comp_(std::move(comp)) {}

    AvlOrderStatisticsTree(const AvlOrderStatisticsTree&) = delete;
    AvlOrderStatisticsTree& operator=(const AvlOrderStatisticsTree&) = delete;

    ~AvlOrderStatisticsTree() { Clear(root_); }

    void Insert(const T& key) { root_ = Insert(root_, key); }

    void Erase(const T& key) { root_ = Erase(root_, key); }

    bool Empty() const { return root_ == nullptr; }

    int Size() const { return Size(root_); }

    T Kth(int k) const { return Kth(root_, k); }

private:
    struct Node {
        T key{};
        int height = 1;
        int subtree_size = 1;
        Node* left = nullptr;
        Node* right = nullptr;

        explicit Node(T k) : key(std::move(k)) {}
    };

    Node* root_;
    Compare comp_;

    static int Height(Node* n) { return n != nullptr ? n->height : 0; }

    static int Size(Node* n) { return n != nullptr ? n->subtree_size : 0; }

    static void Refresh(Node* n) {
        if (n == nullptr) {
            return;
        }
        n->height = 1 + std::max(Height(n->left), Height(n->right));
        n->subtree_size = 1 + Size(n->left) + Size(n->right);
    }

    int BalanceFactor(Node* n) const {
        return n == nullptr ? 0 : Height(n->left) - Height(n->right);
    }

    Node* RotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        Refresh(y);
        Refresh(x);
        return x;
    }

    Node* RotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        Refresh(x);
        Refresh(y);
        return y;
    }

    Node* Balance(Node* n) {
        Refresh(n);
        const int bf = BalanceFactor(n);
        if (bf == 2) {
            if (BalanceFactor(n->left) < 0) {
                n->left = RotateLeft(n->left);
            }
            return RotateRight(n);
        }
        if (bf == -2) {
            if (BalanceFactor(n->right) > 0) {
                n->right = RotateRight(n->right);
            }
            return RotateLeft(n);
        }
        return n;
    }

    Node* Insert(Node* node, const T& key) {
        if (node == nullptr) {
            return new Node(key);
        }
        if (comp_(key, node->key)) {
            node->left = Insert(node->left, key);
        } else if (comp_(node->key, key)) {
            node->right = Insert(node->right, key);
        } else {
            return node;
        }
        return Balance(node);
    }

    static Node* MinNode(Node* n) {
        while (n != nullptr && n->left != nullptr) {
            n = n->left;
        }
        return n;
    }

    Node* Erase(Node* node, const T& key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (comp_(key, node->key)) {
            node->left = Erase(node->left, key);
        } else if (comp_(node->key, key)) {
            node->right = Erase(node->right, key);
        } else {
            if (node->left == nullptr) {
                Node* r = node->right;
                delete node;
                return r;
            }
            if (node->right == nullptr) {
                Node* l = node->left;
                delete node;
                return l;
            }
            Node* succ = MinNode(node->right);
            node->key = succ->key;
            node->right = Erase(node->right, succ->key);
        }
        return Balance(node);
    }

    T Kth(Node* n, int k) const {
        const int left_sz = Size(n->left);
        if (k < left_sz) {
            return Kth(n->left, k);
        }
        if (k == left_sz) {
            return n->key;
        }
        return Kth(n->right, k - left_sz - 1);
    }

    void Clear(Node* n) {
        if (n == nullptr) {
            return;
        }
        Clear(n->left);
        Clear(n->right);
        delete n;
    }
};

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        return 0;
    }
    AvlOrderStatisticsTree<long long, std::less<long long>> tree;

    for (int i = 0; i < n; ++i) {
        long long a = 0;
        int k = 0;
        std::cin >> a >> k;
        if (a > 0) {
            tree.Insert(a);
        } else {
            tree.Erase(-a);
        }
        std::cout << tree.Kth(k) << '\n';
    }
    return 0;
}
