#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>  
#include <functional>

void openURL(const std::string& url) {
    #ifdef _WIN32 // windows
        system(("start " + url).c_str());
    #elif __APPLE__ // apple
        system(("open " + url).c_str());
    #else // linux
        system(("xdg-open " + url).c_str())
    #endif
}

int main() {
    // make a map where "yt" is a key and the value is a function
    std::unordered_map<std::string, std::function<void()>> commands;

    // register commands here
    commands["yt"] = []() { openURL("https://www.youtube.com"); };
    commands["gh"] = []() { openURL("https://github.com"); };

    std::string input;
    std::cout << "HELLO - Type 'exit' to quit\n"; // welcome message

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        // shut down the program if exit is typed
        if (input == "exit") {
            break;
        } 

        // look for a key that matches the user input in the map
        auto it = commands.find(input);
        if (it != commands.end()) { // if it finds something other than end
            it->second(); // call the lambda function associated with that key like "yt" [] () {openURL("youtube.com"); })
        } else { // else print a error
            std::cout << "You entered: " << input << ", thats not a command silly!" "\nTry 'yt' or 'gh'\n";
        }
    }

    return 0;
}
