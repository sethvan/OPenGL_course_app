#ifndef _INCLUDED_LIGHT_H
#define _INCLUDED_LIGHT_H

#include <GL/glew.h>

#include <glm/glm.hpp>

class Light {
   private:
    glm::vec3 colour;
    GLfloat ambientIntensity;

    glm::vec3 direction;
    GLfloat diffuseIntensity;

   public:
    // these colour values are talking aout intensity not amount
    Light( GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f, GLfloat aIntensity = 1.0f,
           GLfloat xDir = 0.0f, GLfloat yDir = -1.0f, GLfloat zDir = 0.0f, GLfloat dIntensity = 1.0f );
    ~Light();

    void useLight( GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
                   GLuint directionLocation );
};

#endif  // _INCLUDED_LIGHT_H
