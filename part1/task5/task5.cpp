/*
 * Задача № 5
 * 5.3. Закраска прямой.
 * На числовой прямой окрасили N отрезков.
 * Известны координаты левого и правого концов каждого отрезка [L_i, R_i]. Найти длину окрашенной части
 * N ≤ 10000. L_i, R_i — целые числа в диапазоне [0, 10^9].
 * 
 * Требования:
 * Необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции. Решение
 * должно поддерживать передачу функции сравнения снаружи.
 *
 * Формат ввода:
 * В первой строке записано количество отрезков.
 * В каждой последующей строке через пробел записаны координаты левого и правого концов отрезка.
 *
 * Формат вывода:
 * Выведите целое число — длину окрашенной части.
 *
 * Скорость работы - O(N log N).
 * Потребляемая память - O(N).
 */

#include <iostream>

struct Segment {
    long long left = 0;
    long long right = 0;
};

struct CompareSegmentByLeft {
    bool operator()(const Segment& a, const Segment& b) const {
        if (a.left != b.left) {
            return a.left < b.left;
        }
        return a.right < b.right;
    }
};

template <typename T, typename Compare>
void MergeSortImpl(T* array, int begin, int end, T* buffer, Compare compare) {
    if (end - begin <= 1) {
        return;
    }
    const int mid = begin + (end - begin) / 2;
    MergeSortImpl(array, begin, mid, buffer, compare);
    MergeSortImpl(array, mid, end, buffer, compare);

    int left_index = begin;
    int right_index = mid;
    int buffer_index = 0;
    while (left_index < mid && right_index < end) {
        if (compare(array[right_index], array[left_index])) {
            buffer[buffer_index] = array[right_index];
            ++right_index;
        } 
        else {
            buffer[buffer_index] = array[left_index];
            ++left_index;
        }
        ++buffer_index;
    }
    while (left_index < mid) {
        buffer[buffer_index] = array[left_index];
        ++left_index;
        ++buffer_index;
    }
    while (right_index < end) {
        buffer[buffer_index] = array[right_index];
        ++right_index;
        ++buffer_index;
    }
    for (int i = 0; i < buffer_index; ++i) {
        array[begin + i] = buffer[i];
    }
}

template <typename T, typename Compare>
void MergeSort(T* array, int count, T* buffer, Compare compare) {
    if (count <= 1) {
        return;
    }
    MergeSortImpl(array, 0, count, buffer, compare);
}

long long UnionLength(const Segment* segments, int count) {
    if (count == 0) {
        return 0;
    }
    long long current_left = segments[0].left;
    long long current_right = segments[0].right;
    long long total = 0;
    for (int i = 1; i < count; ++i) {
        if (segments[i].left <= current_right) {
            if (segments[i].right > current_right) {
                current_right = segments[i].right;
            }
        } 
        else {
            total += current_right - current_left;
            current_left = segments[i].left;
            current_right = segments[i].right;
        }
    }
    total += current_right - current_left;
    return total;
}

int main() {
    int segment_count = 0;
    if (!(std::cin >> segment_count)) {
        return 0;
    }
    if (segment_count < 0) {
        return 0;
    }

    Segment* segments = new Segment[segment_count];
    for (int i = 0; i < segment_count; ++i) {
        std::cin >> segments[i].left >> segments[i].right;
    }

    Segment* merge_buffer = nullptr;
    if (segment_count > 0) {
        merge_buffer = new Segment[segment_count];
    }
    CompareSegmentByLeft compare;
    MergeSort(segments, segment_count, merge_buffer, compare);

    const long long answer = UnionLength(segments, segment_count);
    std::cout << answer << '\n';

    delete[] merge_buffer;
    delete[] segments;
    return 0;
}
