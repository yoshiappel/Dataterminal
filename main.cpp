#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>
#include "nlohmann/json.hpp"
#include <cstdlib>       
#include <filesystem>    
namespace fs = std::filesystem;

using json = nlohmann::json;

fs::path getConfigFilePath() {
#ifdef _WIN32 // on windows try to get the user home directory and then build the path
    const char* homeDrive = std::getenv("HOMEDRIVE");
    const char* homePath = std::getenv("HOMEPATH");
    if (homeDrive && homePath) {
        return fs::path(std::string(homeDrive) + std::string(homePath)) / ".cmdrc";
    }
#else // on linux based machines
    const char* home = std::getenv("HOME");
    if (home) {
        return fs::path(home) / ".cmdrc";
    }
#endif
    return fs::path(".cmdrc"); // fallback to current working directory
}

// get the users json file path
std::string getJsonPathFromConfig() {
    fs::path configPath = getConfigFilePath();

    if (fs::exists(configPath)) {
        std::ifstream in(configPath);
        std::string path;
        std::getline(in, path);
        return path;
    }

    std::cout << "Welcome! It looks like this is your first time running the DataTerminal.\n";
    std::cout << "Please enter the full path to your commands.json file:\n> ";

    std::string jsonPath;
    std::getline(std::cin, jsonPath);

    // remove any quotes
    jsonPath.erase(
        remove( jsonPath.begin(), jsonPath.end(), '\"' ),
        jsonPath.end()
    );

    // Check if file exists before saving
    if (!fs::exists(jsonPath)) {
        std::cerr << "Error: Could not find the json file at " << jsonPath << "\n";
        return "";
    }

    // save it for future runs
    std::ofstream out(configPath);
    out << jsonPath << std::endl;

    return jsonPath;
}

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
    // first load and check the path
    std::string jsonPath = getJsonPathFromConfig();

    if (jsonPath.empty()) {
        std::cerr << "No valid path to commands.json was provided. Exiting.\n";
        return 1;
    }

    // then load and check the commands
    CommandData data = loadJsonCommands(jsonPath);

    if (data.commands.empty()) {
        std::cerr << "No commands were loaded. Exiting.\n";
        return 1;
    }

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
        
        if (input == "--reset") {
            fs::remove(getConfigFilePath());
            std::cout << "Configuration reset. Run the DataTerminal again to reconfigure.\n";
            return 0;
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
