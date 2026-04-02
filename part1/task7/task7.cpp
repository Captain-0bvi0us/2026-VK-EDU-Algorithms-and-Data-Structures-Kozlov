/*
 * Задача 7.2. LSD для long long.
 * Отсортировать массив целых неотрицательных 64-битных чисел по возрастанию.
 * Алгоритм: поразрядная сортировка LSD по байтам.
 *
 * Скорость работы - O(n).
 * Потребляемая память - O(n).
 */

#include <iostream>
#include <utility>

void SortLSD(unsigned long long* data, unsigned long long* buffer, int count) {
    unsigned long long* current = data;
    unsigned long long* other = buffer;
    for (int byte_index = 0; byte_index < 8; ++byte_index) {
        const int shift = byte_index * 8;
        int count_per_digit[256] = {};
        for (int i = 0; i < count; ++i) {
            const unsigned char digit = static_cast<unsigned char>((current[i] >> shift) & 255ULL);
            count_per_digit[digit]++;
        }
        int start[256];
        start[0] = 0;
        for (int digit = 1; digit < 256; ++digit) {
            start[digit] = start[digit - 1] + count_per_digit[digit - 1];
        }
        for (int i = 0; i < count; ++i) {
            const unsigned char digit = static_cast<unsigned char>((current[i] >> shift) & 255ULL);
            other[start[digit]++] = current[i];
        }
        std::swap(current, other);
    }
}

int main() {
    int count = 0;
    if (!(std::cin >> count)) {
        return 0;
    }
    if (count <= 0) {
        return 0;
    }

    unsigned long long* data = new unsigned long long[count];
    unsigned long long* buffer = new unsigned long long[count];
    for (int i = 0; i < count; ++i) {
        std::cin >> data[i];
    }

    SortLSD(data, buffer, count);

    for (int i = 0; i < count; ++i) {
        std::cout << data[i] << ' ';
    }
    std::cout << '\n';

    delete[] buffer;
    delete[] data;
    return 0;
}
