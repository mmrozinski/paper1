#ifndef PAPER1_FILEHELPER_H
#define PAPER1_FILEHELPER_H

#include <iostream>
#include <fstream>
#include <sstream>

class FileHelper {
public:
    static std::string fileToString(const std::string& path) {
        std::ifstream s(path);
        std::stringstream buffer;
        buffer << s.rdbuf();
        return buffer.str();
    }
};

#endif //PAPER1_FILEHELPER_H
