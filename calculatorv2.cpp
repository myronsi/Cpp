#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

int main() {
    string input;
    cout << "Введите дробь в формате a/b: ";
    getline(cin, input);

    size_t slashPos = input.find('/');
    if (slashPos == string::npos) {
        cout << "Ошибка: неверный формат ввода!" << endl;
        return 1;
    }

    string numStr = input.substr(0, slashPos);
    string denomStr = input.substr(slashPos + 1);

    int numerator, denominator;
    try {
        numerator = stoi(numStr);
        denominator = stoi(denomStr);
    } catch (...) {
        cout << "Ошибка: введены некорректные числа!" << endl;
        return 1;
    }

    if (denominator == 0) {
        cout << "Ошибка: деление на ноль!" << endl;
        return 1;
    }

    bool isNegative = (numerator < 0) ^ (denominator < 0);
    numerator = abs(numerator);
    denominator = abs(denominator);

    int integerPart = numerator / denominator;
    int remainder = numerator % denominator;

    if (isNegative) cout << "-";
    cout << integerPart << ".";

    int zeroCounter = 0;
    //constexpr int delay_us = 1; // 1 мс на символ

    while (true) {
        remainder *= 10;
        int digit = remainder / denominator;
        remainder %= denominator;

        cout << digit;
        cout.flush();
        
        // Добавляем задержку
       // this_thread::sleep_for(chrono::microseconds(delay_us));

        if (digit == 0) {
            zeroCounter++;
        } else {
            zeroCounter = 0;
        }

        if (zeroCounter >= 10) {
            break;
        }
    }

    cout << "\nПрограмма завершена: обнаружено 10 нулей подряд" << endl;
    return 0;
}
