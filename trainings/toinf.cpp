#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> stop_flag(false); // Флаг для остановки программы

void check_for_stop() {
    std::cin.get(); // Ожидаем нажатия Enter
    stop_flag = true; // Устанавливаем флаг для остановки
}

int main() {
    std::thread stop_thread(check_for_stop); // Запускаем поток для проверки ввода

    unsigned long long counter = 0;

    while (!stop_flag) { // Цикл работает, пока не установлен флаг остановки
        std::cout << counter << std::endl;
        counter++;
    }

    stop_thread.join(); // Ждем завершения потока
    std::cout << "Программа остановлена!" << std::endl;

    return 0;
}
