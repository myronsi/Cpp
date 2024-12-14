#include <iostream>
using namespace std;

int main() {
    for (int i = 0; i < 5; i++) {
        cout << "For: " << i << endl;
    }

    int i = 0;

    while (i < 5) {
        cout << "While: " << i << endl;
        i++;
    }

    int a = 0;

    do {
        cout << "Do: " << a << endl;
        a++;
    } while (a < 5);


    for (int i = 0; i < 10; i++) {
    if (i == 5) {
        break; // Выход из цикла
    }
    cout << "For: " << i << endl;
    }

    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue; // Пропуск чётных чисел
        }
        cout << "While: " << i << endl;
    }    


    return 0;
}
