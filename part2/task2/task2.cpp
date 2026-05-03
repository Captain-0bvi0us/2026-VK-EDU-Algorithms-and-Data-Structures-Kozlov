/*
 * Задача № 2
 *
 * 2_3. Обход бинарного дерева поиска в порядке post-order (без рекурсии).
 * После числа N вводятся N целых чисел — ключи в порядке вставки в БДП.
 * Правило вставки: если у текущего узла key <= K, спуск вправо, иначе влево.
 * Вывод: post-order. Рекурсия запрещена.
 * Вставка принимает компаратор снаружи; по умолчанию std::less<T>
 * (для задачи: влево при K < key, иначе вправо, дубликаты вправо).
 *
 * Скорость работы - O(N).
 * Потребляемая память - O(N).
 */

#include <functional>
#include <iostream>
#include <stack>
#include <vector>

template <typename T>
struct Node {
    T key{};
    Node* left = nullptr;
    Node* right = nullptr;
};

template <typename T, typename Compare = std::less<T>>
Node<T>* InsertIterative(Node<T>* root, const T& value, Compare comp = Compare{}) {
    if (root == nullptr) {
        Node<T>* created = new Node<T>();
        created->key = value;
        return created;
    }
    Node<T>* cur = root;
    while (true) {
        if (comp(value, cur->key)) {
            if (cur->left == nullptr) {
                cur->left = new Node<T>();
                cur->left->key = value;
                return root;
            }
            cur = cur->left;
        } else {
            if (cur->right == nullptr) {
                cur->right = new Node<T>();
                cur->right->key = value;
                return root;
            }
            cur = cur->right;
        }
    }
}

template <typename T>
std::vector<Node<T>*> PostOrderNodes(Node<T>* root) {
    std::vector<Node<T>*> order;
    if (root == nullptr) {
        return order;
    }
    std::stack<Node<T>*> first;
    std::stack<Node<T>*> second;
    first.push(root);
    while (!first.empty()) {
        Node<T>* node = first.top();
        first.pop();
        second.push(node);
        if (node->left != nullptr) {
            first.push(node->left);
        }
        if (node->right != nullptr) {
            first.push(node->right);
        }
    }
    order.reserve(second.size());
    while (!second.empty()) {
        order.push_back(second.top());
        second.pop();
    }
    return order;
}

template <typename T>
void FreeNodesInPostOrderSequence(const std::vector<Node<T>*>& post) {
    for (Node<T>* node : post) {
        delete node;
    }
}

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        return 0;
    }
    Node<long long>* root = nullptr;
    for (int i = 0; i < n; ++i) {
        long long x = 0;
        std::cin >> x;
        root = InsertIterative(root, x);
    }

    std::vector<Node<long long>*> post = PostOrderNodes(root);
    for (size_t i = 0; i < post.size(); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << post[i]->key;
    }
    std::cout << '\n';

    FreeNodesInPostOrderSequence(post);
    return 0;
}
