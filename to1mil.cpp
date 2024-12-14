#include <iostream>

int main() {
    int count = 1;

    while(true) {
        std::cout << count << std::endl;
        count++;

        if(count == 10000000) {
            break;
        }
    }
    return 0;
}