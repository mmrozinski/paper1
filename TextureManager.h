#ifndef PAPER1_TEXTUREMANAGER_H
#define PAPER1_TEXTUREMANAGER_H

#include <GL/glew.h>

class TextureManager {
private:
    static TextureManager* instance;

    TextureManager();

    unsigned int texture;
public:
    static TextureManager* getInstance();
    void setTextureImage(unsigned char* data, int width, int height, int nrChannels);
    ~TextureManager();
};

#endif //PAPER1_TEXTUREMANAGER_H
