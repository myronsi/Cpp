#include <iostream>
#include <stdexcept>

int main() {
    try {
        double a, b;
        char op;
        std::cout << "Введите выражение (a op b): ";
        std::cin >> a >> op >> b;

        switch (op) {
            case '+': std::cout << a + b; break;
            case '-': std::cout << a - b; break;
            case '*': std::cout << a * b; break;
            case '/': 
                if (b == 0) throw std::runtime_error("Деление на ноль!");
                std::cout << a / b; 
                break;
            default: throw std::invalid_argument("Неверная операция");
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what();
    }
    return 0;
}
