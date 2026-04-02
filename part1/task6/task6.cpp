 /*
 * Задача № 6.4
 * Дано множество целых чисел из [0..10^9] размера n.
 * Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
 * 10-й, 50-й и 90-й процентили.
 *
 * Требования:
 * Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition реализуйте 
 * методом прохода двумя итераторами от конца массива к началу.
 *
 * Скорость работы - O(N).
 * Потребляемая память - O(N).
 */

#include <cmath>
#include <cstring>
#include <iostream>
#include <random>
#include <utility>

struct CompareLongLong{
    bool operator()(long long left_value, long long right_value) const {
        return left_value < right_value;
    }
};

int PercentileRank(int count, int percent) {
    if (count <= 0) {
        return 0;
    }
    const double rank = std::ceil((static_cast<double>(count) + 1.0) * static_cast<double>(percent) / 100.0);
    int k = static_cast<int>(rank);
    if (k < 1) {
        k = 1;
    }
    if (k > count) {
        k = count;
    }
    return k;
}

template <typename T, typename Compare, typename Rng>
int Partition(T* array, int left, int right, Compare compare, Rng& rng) {
    std::uniform_int_distribution<int> pick(left, right);
    const int pivot_index = pick(rng);
    std::swap(array[pivot_index], array[left]);
    const T pivot_value = array[left];
    int i = left - 1;
    int j = right + 1;
    while (true) {
        do {
            --j;
        } while (compare(pivot_value, array[j]));

        do {
            ++i;
        } while (compare(array[i], pivot_value));

        if (i >= j) {
            return j;
        }
        std::swap(array[i], array[j]);
    }
}

template <typename T, typename Compare, typename Rng>
T QuickSelect(T* array, int count, int k_one_based, Compare compare, Rng& rng) {
    const int target_index = k_one_based - 1;
    int left = 0;
    int right = count - 1;
    while (left < right) {
        const int border = Partition(array, left, right, compare, rng);
        if (target_index <= border) {
            right = border;
        } 
        else {
            left = border + 1;
        }
    }
    return array[left];
}

int main() {
    int count = 0;
    if (!(std::cin >> count)) {
        return 0;
    }
    if (count <= 0) {
        return 0;
    }

    long long* original = new long long[count];
    for (int i = 0; i < count; ++i) {
        std::cin >> original[i];
    }

    CompareLongLong compare;
    std::mt19937 generator(std::random_device{}());

    const int rank10 = PercentileRank(count, 10);
    const int rank50 = PercentileRank(count, 50);
    const int rank90 = PercentileRank(count, 90);

    long long* work = new long long[count];
    for (int i = 0; i < count; ++i) {
        work[i] = original[i];
    }    
    const long long value10 = QuickSelect(work, count, rank10, compare, generator);

    for (int i = 0; i < count; ++i) {
        work[i] = original[i];
    }
    const long long value50 = QuickSelect(work, count, rank50, compare, generator);

    for (int i = 0; i < count; ++i) {
        work[i] = original[i];
    }
    const long long value90 = QuickSelect(work, count, rank90, compare, generator);

    std::cout << value10 << '\n' << value50 << '\n' << value90 << '\n';

    delete[] work;
    delete[] original;
    return 0;
}
