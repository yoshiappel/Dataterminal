#include <iostream>
#include <string>
#include <cstdlib>  

int main() {
    std::string input;
    std::cout << "HELLO - Type 'exit' to quit\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        } else if (input == "yt") {
            // open YouTube in default browser
#ifdef _WIN32 // windows
            system("start https://www.youtube.com");
#elif __APPLE__ // apple
            system("open https://www.youtube.com");
#else // linux
            system("xdg-open https://www.youtube.com");
#endif
        } else {
            std::cout << "You entered: " << input << "\n";
        }
    }

    return 0;
}
