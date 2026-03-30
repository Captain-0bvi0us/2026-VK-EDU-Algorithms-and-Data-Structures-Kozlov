#include <iostream>

int FindStartIndex(const int* arr, int count) {
    int left = 0;
    int right = count - 1;

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] > arr[right]) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int main() {
    int count = 0;
    if (!(std::cin >> count)) {
        return 0;
    }

    int* arr = new int[count];
    for (int i = 0; i < count; ++i) {
        std::cin >> arr[i];
    }

    int answer = FindStartIndex(arr, count);
    std::cout << answer << '\n';

    delete[] arr;
    return 0;
}
