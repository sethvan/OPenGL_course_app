#ifndef _INCLUDED_LIGHT_H
#define _INCLUDED_LIGHT_H

#include <GL/glew.h>

#include <glm/glm.hpp>

struct LightingUniVarBindings {
    GLint ambientIntensityLocation = -1;
    GLint ambientColourLocation = -1;
    GLint diffuseIntensityLocation = -1;
    GLint directionLocation = -1;
    GLint positionLocation = -1;
    GLint constantLocation = -1;
    GLint linearLocation = -1;
    GLint exponentLocation = -1;
    GLint edgeLocation = -1;
};

class Light {
   protected:
    glm::vec3 colour;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

   public:
    // these colour values are talking aout intensity not amount
    Light( GLfloat red = 1.0f, GLfloat green = 1.0f, GLfloat blue = 1.0f,
           GLfloat aIntensity = 1.0f, GLfloat dIntensity = 1.0f );

    virtual ~Light() = default;

    virtual void useLight( const LightingUniVarBindings& lighting ) const;
};

#endif  // _INCLUDED_LIGHT_H
