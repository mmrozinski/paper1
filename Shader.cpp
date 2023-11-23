#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    int vertexShader;
    int fragmentShader;

    std::string vertexShaderSource = FileHelper::fileToString(vertexPath);
    std::string fragmentShaderSource = FileHelper::fileToString(fragmentPath);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    shaderSource(vertexShader, vertexShaderSource);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderSource(fragmentShader, fragmentShaderSource);

    glCompileShader(vertexShader);

    int success;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        std::string infoLog;
        GLint totalLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &totalLength);
        infoLog.resize(totalLength);
        glGetShaderInfoLog(vertexShader, totalLength, nullptr, &infoLog[0]);
        std::cout << infoLog << std::endl;
    }

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        std::string infoLog;
        GLint totalLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &totalLength);
        infoLog.resize(totalLength);
        glGetShaderInfoLog(fragmentShader, totalLength, nullptr, &infoLog[0]);
        std::cout << infoLog << std::endl;
    }

    _handle = glCreateProgram();

    glAttachShader(_handle, vertexShader);
    glAttachShader(_handle, fragmentShader);

    glLinkProgram(_handle);

    glGetProgramiv(_handle, GL_LINK_STATUS, &success);
    if (success == 0) {
        std::string infoLog;
        GLint totalLength = 0;
        glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &totalLength);
        infoLog.resize(totalLength);
        glGetProgramInfoLog(_handle, totalLength, nullptr, &infoLog[0]);
        std::cout << infoLog << std::endl;
    }

    glDetachShader(_handle, vertexShader);
    glDetachShader(_handle, fragmentShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
}

void Shader::shaderSource(int shader, const std::string& string) {
    int length = string.size();
    glShaderSource(shader, 1, new GLchar const *[] {string.c_str()}, &length);
}

void Shader::setMatrix4(const std::string& name, Matrix4 &projection) {
    int matrixLocation = glGetUniformLocation(_handle, name.c_str());
    glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, projection[0]);
}

int Shader::getAttribLocation(const std::string& name) {
    return glGetAttribLocation(_handle, name.c_str());
}

void Shader::use() {
    glUseProgram(_handle);
}

Shader::~Shader() {
    glDeleteProgram(_handle);
}
