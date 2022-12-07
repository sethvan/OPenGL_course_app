#ifndef _INCLUDED_TEXTURE_H
#define _INCLUDED_TEXTURE_H

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
   private:
    GLuint textureID;
    int width, height, bitDepth;

    const char* fileLocation;

   public:
    Texture( const char* fileLoc = "" );
    ~Texture();

    void loadTexture();
    void useTexture();
    void clearTexture();
};

#endif  // _INCLUDED_TEXTURE_H
