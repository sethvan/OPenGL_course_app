#ifndef _INCLUDED_TEXTURE_H
#define _INCLUDED_TEXTURE_H

#include <GL/glew.h>
#include <string>
#include "stb_image.h"

class Model;

class Texture {
  private:
   friend class Model;

   GLuint textureID;
   int width, height, bitDepth;

  std::string fileLocation;

  public:
   Texture( const std::string& fileLoc );
   ~Texture();

   bool loadTexture();   // for without alpha channel
   bool loadTextureA();  // for with alpha channel

   void useTexture();
   void clearTexture();
};

#endif  // _INCLUDED_TEXTURE_H
