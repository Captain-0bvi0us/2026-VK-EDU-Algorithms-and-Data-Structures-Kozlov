/*
 * Задача № 1
 * Во всех задачах необходимо использование битовых операций.
 * Использование арифметических операций запрещено.
 * Входное число лежит в диапазоне 0..2^32 -1 и вводится в десятичном виде.
 * 
 * 1.1 Подсчитать кол-во единичных бит в входном числе, стоящих на четных позициях. 
 * Позиции битов нумеруются с 0.
 *
 * Скорость работы - О(1).
 * Потребляемая память - О(1).
 */

 #include <iostream>

 int CountEvenSetBits(unsigned long long num){
     int count = 0;
     
     while (num > 0){
         if(num & 1){
             count++;
         }
         num >>= 2;
     }
     
     return count;
 }
 
 int main(){
     
     unsigned long long num = 0;
     std::cin >> num;
     
     int result = CountEvenSetBits(num);
 
     std::cout << result << '\n';
     
     return 0;
 }