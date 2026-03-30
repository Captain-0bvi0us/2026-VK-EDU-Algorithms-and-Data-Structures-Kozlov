/*
 * Задача № 2
 *
 * 2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. 
 * Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента 
 * массива A, равного или превосходящего B[i]: A[k] >= B[i]. 
 * Если такого элемента нет, выведите n. n, m <= 10000.
 *
 * Требования: Время работы поиска k для каждого элемента B[i]: O(log(k)). 
 * Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для 
 * бинарного поиска размером порядка k с помощью экспоненциального поиска, 
 * а потом уже в нем делать бинарный поиск.
 * 
 * Скорость работы - O(m * log k).
 * Потребляемая память - O(n + m).
 */
 
 #include <iostream>
 #include <algorithm>
 
 int BinarySearch(const int* array_a, int n, int left, int right, int target) {
     int index = n;
     
     while (left <= right) {
         int mid = left + (right - left) / 2;
         
         if (array_a[mid] >= target) {
             index = mid;
             right = mid - 1;   
         } else {  
             left = mid + 1;  
         }
     }
     
     return index;
 }
 
 int ExponentialSearch(const int* array_a, int n, int target) {
     if (n == 0) {
         return 0;
     }
     
     if (array_a[0] >= target) {
         return 0;
     }
     
     int bound = 1;
     while (bound < n && array_a[bound] < target) {
         bound *= 2;
     }
     
     int left = bound / 2;
     int right = std::min(bound, n - 1);
     
     return BinarySearch(array_a, n, left, right, target);
 }
 
 int main() {
     int n = 0;
     int m = 0;
     
     if (!(std::cin >> n >> m)) {
         return 0;
     }
     
     int* array_a = new int[n];
     for (int i = 0; i < n; i++) {
         std::cin >> array_a[i];
     }
     
     int* array_b = new int[m];
     for (int i = 0; i < m; i++) {
         std::cin >> array_b[i];
     }
     
     for (int i = 0; i < m; i++) {
         int result = ExponentialSearch(array_a, n, array_b[i]);
         std::cout << result << " ";
     }
     std::cout << '\n';
     
     delete[] array_a;
     delete[] array_b;
     
     return 0;
 }