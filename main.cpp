#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// define what to do on different OS
void openURL(const std::string& url) {
    #ifdef _WIN32
        system(("start " + url).c_str());
    #elif __APPLE__
        system(("open " + url).c_str());
    #else
        system(("xdg-open " + url).c_str());
    #endif
}

// make a map where the key is (for example, "yt") and the value is a function that opens the associated url (like, youtube.com)
std::unordered_map<std::string, std::function<void()>> loadJsonCommands(const std::string& filename) {
    std::unordered_map<std::string, std::function<void()>> commands;

    std::ifstream file(filename); // open the file
    if (!file) { // if it fails to open the file, notify the user
        std::cerr << "Failed to open " << filename << std::endl;
        return commands;
    }

    json j; // declare the json object
    file >> j; // read the json file into it so { "yt": "youtube.com" } becomes { {"yt", "youtube.com"}}

    // store the key and values in the commands map so the later function can use them
    for (auto& [key, val] : j.items()) {
        if (val.is_string()) {
            std::string url = val.get<std::string>(); // extract the url string 
            commands[key] = [url]() { openURL(url); }; // create a lambda function that captures the URL and calls openURL, then store the lambda function in the commands map with the corresponding key
        }
    }

    return commands; // return the map so the CLI can use it to respond to commands
}

int main() {
    auto commands = loadJsonCommands("commands.json"); // Load commands

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
