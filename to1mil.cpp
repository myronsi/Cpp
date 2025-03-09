#include <iostream>

int main() {
    int count = 1;

    while(true) {
        std::cout << count << std::endl;
        count++;

        if(count == 100001) {
            break;
        }
    }
    return 0;
}
