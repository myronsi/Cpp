#include <iostream>
using namespace std;

int main() {
    int number = 7;

    if (number > 10) {
        cout << "Число больше 10" << endl;
    } else if (number > 5) {
        cout << "Число больше 5, но меньше или равно 10" << endl;
    } else {
        cout << "Число меньше или равно 5" << endl;
    }


    int age = 18;
    string access = (age >= 18) ? "Доступ разрешён" : "Доступ запрещён";

    cout << access << endl;


    return 0;
}
