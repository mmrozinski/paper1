#ifndef PAPER1_SHADER_H
#define PAPER1_SHADER_H

#include <iostream>
#include <GL/glew.h>
#include "FileHelper.h"
#include "Matrix4.h"

class Shader {
private:
    int _handle;

    static void shaderSource(int shader, const std::string& string);
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void setMatrix4(const std::string& name, Matrix4 &projection);
    int getAttribLocation(const std::string& name);
    void use();
    ~Shader();
};


#endif //PAPER1_SHADER_H
