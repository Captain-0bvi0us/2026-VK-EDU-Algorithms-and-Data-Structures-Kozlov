/*
 * Задача № 4
 * 4.2. Топ K пользователей из лога.
 * Имеется лог-файл, в котором хранятся пары для N пользователей 
 * (Идентификатор пользователя, посещаемость сайта).
 * Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, 
 * и выводит их в порядке возрастания посещаемости.
 * Количество заходов и идентификаторы пользователей не повторяются.
 *
 * Требования:
 * Время работы O(N * logK).
 * Куча должна быть реализована в виде шаблонного класса.
 * Решение должно поддерживать передачу функции сравнения снаружи.
 * Куча должна быть динамической.
 *
 * Формат ввода:
 * Сначала вводятся N и K, затем пары (Идентификатор пользователя, посещаемость сайта).
 *
 * Формат вывода:
 * Идентификаторы пользователей в порядке возрастания посещаемости.
 *
 * Скорость работы - O(N log K).
 * Потребляемая память - O(K).
 */

#include <algorithm>
#include <iostream>
#include <utility>

struct UserRecord {
    int user_id = 0;
    int visit_count = 0;
};

struct CompareByVisitCountAsc {
    bool operator()(const UserRecord& left, const UserRecord& right) const {
        return left.visit_count < right.visit_count;
    }
};

template <typename T, typename Compare>
class BinaryHeap {
public:
    explicit BinaryHeap(int capacity, Compare compare)
        : data_(nullptr),
          size_(0),
          capacity_(capacity),
          compare_(std::move(compare)) {
        if (capacity_ > 0) {
            data_ = new T[capacity_];
        }
    }

    ~BinaryHeap() { delete[] data_; }

    BinaryHeap(const BinaryHeap&) = delete;
    BinaryHeap& operator=(const BinaryHeap&) = delete;

    int Size() const { return size_; }
    bool Empty() const { return size_ == 0; }

    const T& Top() const {
        return data_[0];
    }

    void Push(const T& value) {
        data_[size_] = value;
        ++size_;
        SiftUp(size_ - 1);
    }

    void Pop() {
        data_[0] = data_[size_ - 1];
        --size_;
        if (size_ > 0) {
            SiftDown(0);
        }
    }

private:
    void SiftUp(int index) {
        while (index > 0) {
            const int parent = (index - 1) / 2;
            if (!compare_(data_[index], data_[parent])) {
                break;
            }
            std::swap(data_[index], data_[parent]);
            index = parent;
        }
    }

    void SiftDown(int index) {
        while (true) {
            const int left_child = 2 * index + 1;
            if (left_child >= size_) {
                break;
            }
            int smallest = left_child;
            const int right_child = left_child + 1;
            if (right_child < size_ && compare_(data_[right_child], data_[left_child])) {
                smallest = right_child;
            }
            if (!compare_(data_[smallest], data_[index])) {
                break;
            }
            std::swap(data_[index], data_[smallest]);
            index = smallest;
        }
    }

    T* data_;
    int size_;
    int capacity_;
    Compare compare_;
};

void InsertUser(BinaryHeap<UserRecord, CompareByVisitCountAsc>& heap, int k, const UserRecord& record) {
    if (heap.Size() < k) {
        heap.Push(record);
    } 
    else if (record.visit_count > heap.Top().visit_count) {
        heap.Pop();
        heap.Push(record);
    }
}

int main() {
    int user_count = 0;
    int top_k = 0;
    if (!(std::cin >> user_count >> top_k)) {
        return 0;
    }
    if (top_k == 0) {
        std::cout << '\n';
        return 0;
    }

    CompareByVisitCountAsc compare;
    BinaryHeap<UserRecord, CompareByVisitCountAsc> heap(top_k, compare);

    for (int i = 0; i < user_count; ++i) {
        int user_id = 0;
        int visits = 0;
        std::cin >> user_id >> visits;
        const UserRecord record{user_id, visits};
        InsertUser(heap, top_k, record);
    }

    while (!heap.Empty()) {
        const UserRecord top = heap.Top();
        heap.Pop();
        std::cout << top.user_id << " ";
    }
    std::cout << '\n';

    return 0;
}
