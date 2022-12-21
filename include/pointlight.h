#ifndef _INCLUDED_POINTLIGHT_H
#define _INCLUDED_POINTLIGHT_H

#include "light.h"

class PointLight : public Light {
   protected:
    glm::vec3 position;

    // for attenuation = 0.0f, L / (ax^2 + bx + c)
    // L = light, x = distance, a = exponent, b = linear, c = constant
    GLfloat constant, linear, exponent;

   public:
    // clang-format off
    PointLight( GLfloat red, GLfloat green, GLfloat blue,
                GLfloat aIntensity, GLfloat dIntensity, 
                GLfloat xPos = 0.0f, GLfloat yPos = 0.0f, GLfloat zPos = 0.0f,
                GLfloat con = 1.0f, GLfloat lin = 0.0f, GLfloat exp = 0.0f);
    // clang-format on

    void useLight( const LightingUniVarBindings& lighting ) const override;
};

#endif  // _INCLUDED_POINTLIGHT_H