#include <iostream>
#include <string>

class Stack {
public:
    explicit Stack(int max_depth)
        : buffer_(max_depth > 0 ? new char[max_depth] : nullptr),
          size_(0) {}

    ~Stack() { delete[] buffer_; }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void Push(char value) {
        buffer_[size_] = value;
        ++size_;
    }

    void Pop() { --size_; }

    bool Empty() const { return size_ == 0; }

private:
    char* buffer_;
    int size_;
};

bool IsCorrect(const std::string& line) {
    if (line.empty()) {
        return true;
    }

    Stack stack(static_cast<int>(line.size()));
    for (char symbol : line) {
        if (symbol == '(') {
            stack.Push(symbol);
        } 
        else {
            if (stack.Empty()) {
                return false;
            }
            stack.Pop();
        }
    }
    return stack.Empty();
}

int main() {
    std::string str;
    std::cin >> str;

    if (IsCorrect(str)) {
        std::cout << "YES\n";
    } 
    else {
        std::cout << "NO\n";
    }
    return 0;
}
