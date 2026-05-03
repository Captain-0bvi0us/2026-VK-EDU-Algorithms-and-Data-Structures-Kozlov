#include <iostream>

struct Node {
    int value = 0;
    Node* left = nullptr;
    Node* right = nullptr;

    explicit Node(int v) : value(v) {}
};

Node* Insert(Node* root, int x) {
    if (root == nullptr) {
        return new Node(x);
    }
    if (x < root->value) {
        root->left = Insert(root->left, x);
    } else {
        root->right = Insert(root->right, x);
    }
    return root;
}

bool AllEqualTo(const Node* node, int ref) {
    if (node == nullptr) {
        return true;
    }
    if (node->value != ref) {
        return false;
    }
    return AllEqualTo(node->left, ref) && AllEqualTo(node->right, ref);
}

void FreeTree(Node* node) {
    if (node == nullptr) {
        return;
    }
    FreeTree(node->left);
    FreeTree(node->right);
    delete node;
}

int main() {
    Node* root = nullptr;
    int x = 0;
    while (std::cin >> x) {
        root = Insert(root, x);
    }

    if (root == nullptr) {
        std::cout << 1 << '\n';
        return 0;
    }

    const int same = AllEqualTo(root, root->value) ? 1 : 0;
    std::cout << same << '\n';

    FreeTree(root);
    return 0;
}
