#include "ConfigLoader.h"

ConfigLoader* ConfigLoader::instance = nullptr;

ConfigLoader::ConfigLoader() {
    configFile = std::ifstream("config.txt");

    if (configFile.is_open()) {
        std::string line;

        while (std::getline(configFile, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

            if (line[0] == '#' || line.empty()) {
                continue;
            }

            auto delimiterPos = line.find('=');
            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            values[name] = value;
        }
    }
}

void ConfigLoader::validateNameInValues(const std::string& name) const {
    if (!values.contains(name)) {
        throw std::invalid_argument("Key not found in config!");
    }
}

ConfigLoader* ConfigLoader::getInstance() {
    if (instance == nullptr) {
        instance = new ConfigLoader();
    }

    return instance;
}

float ConfigLoader::getParameterFloat(const std::string& name) const {
    validateNameInValues(name);

    return std::stof(values.at(name));
}

int ConfigLoader::getParameterInt(const std::string& name) const {
    if (!values.contains(name)) {
        throw std::invalid_argument("Key not found in config!");
    }

    return std::stoi(values.at(name));
}
