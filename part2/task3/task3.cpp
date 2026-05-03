/*
 * Задача № 3
 *
 * B-дерево минимальной степени t. На входе t и последовательность ключей.
 * Вывести дерево по уровням: каждый слой с новой строки, слева направо по узлам,
 * в каждом узле — ключи в порядке хранения.
 * Шаблонный класс, компаратор передаётся снаружи.
 * Ключи в условии: 0 .. 2^32−1.
 *
 * Скорость вставки одного ключа - O(log n) по числу узлов на пути; всего O(n log n).
 * Память - O(n).
 */

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class BTree {
public:
    explicit BTree(int minimum_degree, Compare comp = Compare{})
        : t_(minimum_degree), root_(nullptr), comp_(std::move(comp)) {}

    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;

    ~BTree() { Clear(root_); }

    void Insert(const T& key) {
        if (root_ == nullptr) {
            root_ = new Node();
            root_->leaf = true;
            root_->keys.push_back(key);
            return;
        }
        if (static_cast<int>(root_->keys.size()) == 2 * t_ - 1) {
            Node* new_root = new Node();
            new_root->leaf = false;
            new_root->children.push_back(root_);
            root_ = new_root;
            SplitChild(new_root, 0);
        }
        InsertNonFull(root_, key);
    }

    std::vector<std::vector<T>> LevelsByBreadth() const {
        std::vector<std::vector<T>> levels;
        if (root_ == nullptr) {
            return levels;
        }
        std::queue<Node*> q;
        q.push(root_);
        while (!q.empty()) {
            const size_t level_size = q.size();
            std::vector<T> level_keys;
            level_keys.reserve(level_size * static_cast<size_t>(2 * t_));
            for (size_t i = 0; i < level_size; ++i) {
                Node* u = q.front();
                q.pop();
                for (const T& k : u->keys) {
                    level_keys.push_back(k);
                }
                if (!u->leaf) {
                    for (Node* c : u->children) {
                        q.push(c);
                    }
                }
            }
            levels.push_back(std::move(level_keys));
        }
        return levels;
    }

private:
    struct Node {
        bool leaf = true;
        std::vector<T> keys;
        std::vector<Node*> children;
    };

    int t_;
    Node* root_;
    Compare comp_;

    void Clear(Node* x) {
        if (x == nullptr) {
            return;
        }
        if (!x->leaf) {
            for (Node* c : x->children) {
                Clear(c);
            }
        }
        delete x;
    }

    size_t ChildIndex(const Node* x, const T& key) const {
        size_t i = 0;
        while (i < x->keys.size() && comp_(x->keys[i], key)) {
            ++i;
        }
        return i;
    }

    void SplitChild(Node* parent, size_t i) {
        Node* y = parent->children[i];
        Node* z = new Node();
        z->leaf = y->leaf;
        z->keys.assign(y->keys.begin() + t_, y->keys.end());
        const T median = y->keys[t_ - 1];
        y->keys.resize(t_ - 1);
        if (!y->leaf) {
            z->children.assign(y->children.begin() + t_, y->children.end());
            y->children.resize(t_);
        }
        parent->keys.insert(parent->keys.begin() + i, median);
        parent->children.insert(parent->children.begin() + i + 1, z);
    }

    void InsertNonFull(Node* x, const T& key) {
        if (x->leaf) {
            auto pos = std::lower_bound(x->keys.begin(), x->keys.end(), key, comp_);
            x->keys.insert(pos, key);
            return;
        }
        size_t i = ChildIndex(x, key);
        if (static_cast<int>(x->children[i]->keys.size()) == 2 * t_ - 1) {
            SplitChild(x, i);
            if (comp_(x->keys[i], key)) {
                ++i;
            }
        }
        InsertNonFull(x->children[i], key);
    }
};

int main() {
    int t = 0;
    if (!(std::cin >> t)) {
        return 0;
    }
    if (t < 2) {
        return 0;
    }

    BTree<uint32_t> tree(t);
    uint32_t x = 0;
    while (std::cin >> x) {
        tree.Insert(x);
    }
    const std::vector<std::vector<uint32_t>> levels = tree.LevelsByBreadth();
    for (size_t row = 0; row < levels.size(); ++row) {
        if (row > 0) {
            std::cout << '\n';
        }
        for (size_t col = 0; col < levels[row].size(); ++col) {
            if (col > 0) {
                std::cout << ' ';
            }
            std::cout << levels[row][col];
        }
    }
    if (!levels.empty()) {
        std::cout << '\n';
    }
    return 0;
}
