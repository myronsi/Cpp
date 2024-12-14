#include <iostream>
using namespace std;

int main() {

    int i;
    int b;

    std::cout << "Введите желаемое число: ";
    std::cin >> i;    


    for (int a = 1; a <= i; a++) {
        cout << "For: " << a << endl;

    }
    
    return 0;
}