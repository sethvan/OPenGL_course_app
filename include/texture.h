#ifndef _INCLUDED_TEXTURE_H
#define _INCLUDED_TEXTURE_H

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
   private:
    GLuint textureID;
    int width, height, bitDepth;

    char* fileLocation;

   public:
    Texture();
    Texture( char* fileLoc = "" );
    ~Texture();

    void loadTexture();
    void useTexture();
    void clearTexture();
};

#endif  // _INCLUDED_TEXTURE_H
