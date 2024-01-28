#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager *TextureManager::getInstance() {
    if (instance == nullptr) {
        instance = new TextureManager();
    }

    return instance;
}

TextureManager::~TextureManager() {
    glDeleteTextures(1, &texture);
}

void TextureManager::setTextureImage(unsigned char *data, int width, int height, int nrChannels) {
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void TextureManager::bindTexture() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}
