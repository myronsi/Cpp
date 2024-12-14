#include <iostream>  // Подключение библиотеки для ввода/вывода
#include <cstdlib> // для getenv

int main() {

    // Объявление переменных
    //int age;               // Целое число
    //double height;         // Число с плавающей точкой
    //td::string name;      // Строка
    const char* user = getenv("USER"); // Получаем значение переменной USER   

    // Ввод данных
    //std::cout << "Введите ваше имя: ";
    //std::cin >> name;  // Ввод строки (без пробелов)

    //std::cout << "Введите ваш возраст: ";
    //std::cin >> age;

    //std::cout << "Введите ваш рост (в метрах): ";
    //std::cin >> height;

    // Вывод данных
    std::cout << "Добро пожаловать " << user << "\n";
    //std::cout << "Ваше имя: " << name << "\n";
    //std::cout << "Ваш возраст: " << age << " лет\n";
    //std::cout << "Ваш рост: " << height << " метров\n";

    return 0;  // Успешное завершение программы
}
