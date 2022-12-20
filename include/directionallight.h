#ifndef _INCLUDED_DIRECTIONALLIGHT_H
#define _INCLUDED_DIRECTIONALLIGHT_H

#include "light.h"

class DirectionalLight : public Light {
   private:
    glm::vec3 direction;

   public:
    DirectionalLight( GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
                      GLfloat dIntensity, GLfloat xDir = 0.0f, GLfloat yDir = -1.0f,
                      GLfloat zDir = 0.0f );

    void useLight( const LightingUniVarBindings& lighting ) const override;
};

#endif  // _INCLUDED_DIRECTIONALLIGHT_H