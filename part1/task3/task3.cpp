/*
 * Задача № 3
 * 3_1. Реализовать очередь с динамическим зацикленным буфером (на основе динамического массива).
 * Обрабатывать команды push back и pop front.
 * 
 * Формат ввода:
 * В первой строке количество команд n. n <= 1000000.
 * a = 2 - pop front (b - ожидаемое значение, -1 если пусто)
 * a = 3 - push back (b - значение для добавления)
 * 
 * Скорость работы - O(1).
 * Потребляемая память - O(k), где k - максимальное количество элементов в очереди.
 */

 #include <iostream>

 class Queue {
 private:
     int* buffer_;      
     int size_;     
     int count_;        
     int head_ind_;   
     int tail_ind_;
 
     void Resize() {
         int new_size = size_ * 2;
         int* new_buffer = new int[new_size];
 
         for (int i = 0; i < count_; i++) {
             int old_ind = (head_ind_ + i) % size_;
             new_buffer[i] = buffer_[old_ind];
         }
 
         delete[] buffer_;        
         buffer_ = new_buffer;    
         size_ = new_size;
         head_ind_ = 0;
         tail_ind_ = count_;
     }
 
 public:
     Queue() {
         size_ = 4;
         buffer_ = new int[size_];
         count_ = 0;
         head_ind_ = 0;
         tail_ind_ = 0;
     }
 
     ~Queue() {
         delete[] buffer_;
     }
 
     Queue(const Queue& other) {
         size_ = other.size_;
         count_ = other.count_;
         head_ind_ = other.head_ind_;
         tail_ind_ = other.tail_ind_;
         
         buffer_ = new int[size_];
         for (int i = 0; i < size_; i++) {
             buffer_[i] = other.buffer_[i];
         }
     }
 
     Queue& operator=(const Queue& other) {
         if (this == &other) {
             return *this;
         }
         
         int* new_buffer = new int[other.size_];
         for (int i = 0; i < other.size_; i++) {
             new_buffer[i] = other.buffer_[i];
         }
         
         delete[] buffer_;
         buffer_ = new_buffer;
         size_ = other.size_;
         count_ = other.count_;
         head_ind_ = other.head_ind_;
         tail_ind_ = other.tail_ind_;
         
         return *this;
     }
 
     void PushBack(int value) {
         if (count_ == size_) {
             Resize();
         }
         
         buffer_[tail_ind_] = value;
         
         tail_ind_ = (tail_ind_ + 1) % size_;
         
         count_++;
     }
 
     int PopFront() {
         if (count_ == 0) {
             return -1;
         }
         
         int result = buffer_[head_ind_];
 
         head_ind_ = (head_ind_ + 1) % size_;
         
         count_--;
         
         return result;
     }
 };
 
 int main() {
     int n = 0;
     if (!(std::cin >> n)) {
         return 0;
     }
 
     Queue queue;
     bool correct = true;
 
     for (int i = 0; i < n; ++i) {
         int command = 0;
         int value = 0;
         std::cin >> command >> value;
 
         if (command == 3) {
             queue.PushBack(value);
         } 
         else if (command == 2) {
             int actual_value = queue.PopFront();
             
             if (actual_value != value) {
                 correct = false;
             }
         }
     }
 
     if (correct) {
         std::cout << "YES\n";
     } else {
         std::cout << "NO\n";
     }
 
     return 0;
 }