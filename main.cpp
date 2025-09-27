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

    // open the file then read the json file into it so { "yt": "youtube.com" } becomes { {"yt", "youtube.com"}}
    std::ifstream file(filename); 
    if (!file) { 
        std::cerr << "Failed to open " << filename << std::endl;
        return commands;
    }

    json j; 
    file >> j; 

    // store the key and values in the commands map so the later function can use them also check the type
    for (auto& [key, val] : j.items()) {
        if (!val.is_object()) continue;

        std::string type = val.value("type", "");
        
        if (type == "url") {
            std::string url = val.value("value", "");
            commands[key] = [url]() { openURL(url); };
        } else if (type == "steam") {
            int appId = val.value("value", 0);
            commands[key] = [appId]() {
                std::string command = "start steam://run/" + std::to_string(appId);
                system(command.c_str());
            };
        } else {
            std::cerr << "Unknown command type for key: " << key << std::endl;
        }
    }

    return commands; 
}

int main() {
    auto commands = loadJsonCommands("C:/Users/YOURNAME/Documents/Dev/DataTerminalCMD/commands.json"); 

    std::string input;
    std::cout << "Welcome to the best cli in the world! - Type 'exit' to quit\n"; 

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        } 

        // look for a key that matches the user input in the map
        auto it = commands.find(input);
        if (it != commands.end()) { 
            it->second(); // call the lambda function associated with that key like "yt" [] () {openURL("youtube.com"); })
        } else { 
            std::cout << "You entered: " << input << ", thats not a command silly!" "\nTry 'yt' or 'gh'\n";
        }
    }

    return 0;
}
