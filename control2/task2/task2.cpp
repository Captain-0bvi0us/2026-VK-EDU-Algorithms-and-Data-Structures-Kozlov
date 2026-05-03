#include <algorithm>
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

int MinNodesRootToLeaf(const Node* node) {
    if (node == nullptr) {
        return 0;
    }
    if (node->left == nullptr && node->right == nullptr) {
        return 1;
    }
    if (node->left == nullptr) {
        return 1 + MinNodesRootToLeaf(node->right);
    }
    if (node->right == nullptr) {
        return 1 + MinNodesRootToLeaf(node->left);
    }
    return 1 + std::min(MinNodesRootToLeaf(node->left), MinNodesRootToLeaf(node->right));
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
        std::cout << 0 << '\n';
        return 0;
    }

    std::cout << MinNodesRootToLeaf(root) << '\n';
    FreeTree(root);
    return 0;
}
