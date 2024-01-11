#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

class ConfigLoader {
    static ConfigLoader* instance;
    ConfigLoader();

    std::ifstream configFile;

    std::map<const std::string, std::string> values;

    void validateNameInValues(const std::string& name) const;
public:
    static ConfigLoader* getInstance();

    float getParameterFloat(const std::string& name) const;
    int getParameterInt(const std::string& name) const;
};



#endif //CONFIGLOADER_H
