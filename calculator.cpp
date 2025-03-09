#include <iostream>

double degree(double base, int exp) {
    double result = 1.0;
    bool isNegativeExp = exp < 0;

    if (isNegativeExp) {
        exp = -exp;  // Делаем степень положительной
    }

    for (int i = 0; i < exp; ++i) {
        result *= base;
    }

    if (isNegativeExp) {
        result = 1.0 / result;  // Обратное значение для отрицательной степени
    }

    return result;
}

int main() {
    long long num1;
    long long num2;
    char Operator;

    std::cout << "Введите первое число" << std::endl;
    std::cin >> num1;

    std::cout << "Введите второе число" << std::endl;
    std::cin >> num2;

    std::cout << "Выберите операцию: +, -, /, *, ^" << std::endl;
    std::cin >> Operator;  

    if (Operator == '+') {
        std::cout << "Результат: " << num1 + num2 << std::endl;
    }
    else if (Operator == '-') {
        std::cout << "Результат: " << num1 - num2 << std::endl;
    }
    else if (Operator == '/') {
        std::cout << "Результат: " << num1 / num2 << std::endl;
    }
    else if (Operator == '*') {
        std::cout << "Результат: " << num1 * num2 << std::endl;
    }
    else if (Operator == '^') {
        // Преобразуем num2 в int для возведения в степень
        std::cout << num1 << " в степени " << static_cast<int>(num2) << " = " << degree(num1, static_cast<int>(num2)) << std::endl;
    }  
    else {
        std::cout << "Некорректная операция" << std::endl;
    }

    return 0;
}
