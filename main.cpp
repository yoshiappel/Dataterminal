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

// create a struct for easy access
struct CommandData {
    std::unordered_map<std::string, std::function<void()>> commands;
    json commandsJson;
};

// load the json using the given json file
CommandData loadJsonCommands(const std::string& filename) {
    CommandData data;

    // open the file then read the json file into it so { "yt": "youtube.com" } becomes { {"yt", "youtube.com"}}
    std::ifstream file(filename); 
    if (!file) { 
        std::cerr << "Failed to open " << filename << std::endl;
        return data;
    }

    file >> data.commandsJson;

    // store the key and values in the commands map so the later function can use them also check the type
    for (auto& [key, val] : data.commandsJson.items()) {
        if (!val.is_object()) continue;

        std::string type = val.value("type", "");
        
        if (type == "url") {
            std::string url = val.value("value", "");
            data.commands[key] = [url]() { openURL(url); };
        } else if (type == "steam") {
            int appId = val.value("value", 0);
            data.commands[key] = [appId]() {
                std::string command = "start steam://run/" + std::to_string(appId);
                system(command.c_str());
            };
        } else {
            std::cerr << "Unknown command type for key: " << key << std::endl;
        }
    }

    return data; 
}

int main() {
    CommandData data = loadJsonCommands("YOURLOCATION/commands.json");

    std::string input;
    std::cout << "Welcome to the best cli in the world! - Type 'help' to list commands or 'exit' to quit\n"; 

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "help") {
            std::cout << "Available commands:\n";
            for (auto it = data.commandsJson.begin(); it != data.commandsJson.end(); ++it) {
                std::cout << "- " << it.key() << "\n";
            }
            continue; // skip the rest of the loop or it will display a not a command warning
        }

        if (input == "exit") {
            break;
        } 
        

        // look for a key that matches the user input in the map
        auto it = data.commands.find(input);
        if (it != data.commands.end()) { 
            it->second(); // call the lambda function associated with that key like "yt" [] () {openURL("youtube.com"); })
        } else { 
            std::cout << "You entered: " << input << ", thats not a command silly!" "\nTry 'help' for a list of commands\n";
        }
    }

    return 0;
}
