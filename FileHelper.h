#ifndef PAPER1_FILEHELPER_H
#define PAPER1_FILEHELPER_H

#include <fstream>

class FileHelper {
public:
    static std::string fileToString(const std::string& path) {
        if (path == "shader.vert") {
            return R"(
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTextureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec3 ObjectColor;
out vec2 TextureCoordinate;

void main()
{
    gl_Position = vec4(aPosition, 1.0) * model * view * projection;
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = aNormal;
    ObjectColor = aColor;
    TextureCoordinate = aTextureCoordinate;
}
)";
        } else if (path == "shader.frag") {
            return
            R"(
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 ObjectColor;
in vec2 TextureCoordinate;

uniform sampler2D blockTexture;

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

vec3 lightPos = vec3(100.0f, 100.0f, 100.0f);

void main()
{
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);

    vec3 lightDir = normalize(vec3(1.0f, 1.5f, 1.5f));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    vec4 textureColor = texture(blockTexture, TextureCoordinate);

    vec3 result = (ambient + diffuse) * ObjectColor;

    FragColor = textureColor * vec4(result, 1.0);
}
)";
        }
        return "";
    }
};

#endif //PAPER1_FILEHELPER_H
