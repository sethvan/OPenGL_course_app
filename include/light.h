#ifndef _INCLUDED_LIGHT_H
#define _INCLUDED_LIGHT_H

#include <GL/glew.h>

#include <glm/glm.hpp>

class Light {
   private:
    glm::vec3 colour;
    GLfloat ambientIntensity;

   public:
    // these colour values are talking aout intensity not amount
    Light( GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat aIntensity = 1.0f );
    ~Light();

    void useLight( GLfloat ambientIntensityLocation, GLfloat ambientColourLocation );
};

#endif  // _INCLUDED_LIGHT_H
