#include <iostream>
#include <string>
#include <unordered_set>

struct Node {
    std::string product_name;
    int color = 0;
    int size = 0;

    bool operator==(const Node& other) const noexcept {
        return product_name == other.product_name && color == other.color && size == other.size;
    }
};

namespace std {
    template <>
    struct hash<Node> {
        std::size_t operator()(const Node& node) const noexcept {
            const std::size_t h1 = std::hash<std::string>{}(node.product_name);
            const std::size_t h2 = std::hash<int>{}(node.color);
            const std::size_t h3 = std::hash<int>{}(node.size);
            std::size_t h = h1;
            h ^= h2 + 0x9E3779B9 + (h << 6) + (h >> 2);
            h ^= h3 + 0x9E3779B9 + (h << 6) + (h >> 2);
            return h;
        }
    };
}  

int main() {
    std::unordered_set<Node> table;

    char op = 0;
    while (std::cin >> op) {
        Node n;
        std::cin >> n.product_name >> n.color >> n.size;

        if (op == '+') {
            if (table.insert(n).second) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        } else if (op == '-') {
            if (table.erase(n) > 0) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        } else if (op == '?') {
            if (table.find(n) != table.end()) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        }
    }
    return 0;
}
