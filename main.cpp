#include <iostream>
#include <string>

int main() {
    std::string input;
    std::cout << "HELLO - Type 'exit' eto quit\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        std::cout << "You entered: " << input << "\n";
    }

    return 0;
}