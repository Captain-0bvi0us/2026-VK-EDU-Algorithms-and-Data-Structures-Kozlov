#include <algorithm>
#include <iostream>
#include <string>

struct Node {
    int key = 0;
    int height = 1;
    Node* left = nullptr;
    Node* right = nullptr;
};

int Height(const Node* n) {
    return n != nullptr ? n->height : 0;
}

void RecalcHeight(Node* n) {
    if (n == nullptr) {
        return;
    }
    n->height = 1 + std::max(Height(n->left), Height(n->right));
}

int BalanceFactor(const Node* n) {
    return n == nullptr ? 0 : Height(n->left) - Height(n->right);
}

Node* RotRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    RecalcHeight(y);
    RecalcHeight(x);
    return x;
}

Node* RotLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    RecalcHeight(x);
    RecalcHeight(y);
    return y;
}

Node* BalanceNode(Node* n) {
    RecalcHeight(n);
    const int bf = BalanceFactor(n);
    if (bf == 2) {
        if (BalanceFactor(n->left) < 0) {
            n->left = RotLeft(n->left);
        }
        return RotRight(n);
    }
    if (bf == -2) {
        if (BalanceFactor(n->right) > 0) {
            n->right = RotRight(n->right);
        }
        return RotLeft(n);
    }
    return n;
}

Node* MinNodeInSubtree(Node* n) {
    while (n != nullptr && n->left != nullptr) {
        n = n->left;
    }
    return n;
}

Node* Insert(Node* node, int key) {
    if (node == nullptr) {
        Node* t = new Node;
        t->key = key;
        return t;
    }
    if (key < node->key) {
        node->left = Insert(node->left, key);
    } else if (key > node->key) {
        node->right = Insert(node->right, key);
    } else {
        return node;
    }
    return BalanceNode(node);
}

Node* DeleteKey(Node* node, int key) {
    if (node == nullptr) {
        return nullptr;
    }
    if (key < node->key) {
        node->left = DeleteKey(node->left, key);
    } else if (key > node->key) {
        node->right = DeleteKey(node->right, key);
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
        Node* succ = MinNodeInSubtree(node->right);
        node->key = succ->key;
        node->right = DeleteKey(node->right, succ->key);
    }
    return BalanceNode(node);
}

bool Exists(const Node* node, int key) {
    while (node != nullptr) {
        if (key == node->key) {
            return true;
        }
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return false;
}

const Node* NextKey(const Node* node, int x) {
    const Node* res = nullptr;
    const Node* cur = node;
    while (cur != nullptr) {
        if (cur->key > x) {
            res = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return res;
}

const Node* PrevKey(const Node* node, int x) {
    const Node* res = nullptr;
    const Node* cur = node;
    while (cur != nullptr) {
        if (cur->key < x) {
            res = cur;
            cur = cur->right;
        } else {
            cur = cur->left;
        }
    }
    return res;
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
    std::string cmd;
    int x = 0;

    while (std::cin >> cmd) {
        if (cmd == "insert") {
            std::cin >> x;
            root = Insert(root, x);
        } else if (cmd == "delete") {
            std::cin >> x;
            root = DeleteKey(root, x);
        } else {
            std::cin >> x;
            if (cmd == "exists") {
                if (Exists(root, x)) {
                    std::cout << "true\n";
                } else {
                    std::cout << "false\n";
                }
            } else if (cmd == "next") {
                const Node* n = NextKey(root, x);
                if (n == nullptr) {
                    std::cout << "none\n";
                } else {
                    std::cout << n->key << '\n';
                }
            } else if (cmd == "prev") {
                const Node* p = PrevKey(root, x);
                if (p == nullptr) {
                    std::cout << "none\n";
                } else {
                    std::cout << p->key << '\n';
                }
            }
        }
    }

    FreeTree(root);
    return 0;
}
