#ifndef _INCLUDED_SPOTLIGHT_H
#define _INCLUDED_SPOTLIGHT_H

#include "pointlight.h"

class SpotLight : public PointLight {
   private:
    glm::vec3 direction;

    GLfloat edge, procEdge /*cosine of edge i radians*/;

   public:
    // clang-format off
    SpotLight( GLfloat red, GLfloat green, GLfloat blue,
                GLfloat aIntensity, GLfloat dIntensity, 
                GLfloat xPos = 0.0f, GLfloat yPos = 0.0f, GLfloat zPos = 0.0f,
                GLfloat xDir = 0.0f, GLfloat yDir = -1.0f, GLfloat zDir = 0.0f,
                GLfloat con = 1.0f, GLfloat lin = 0.0f, GLfloat exp = 0.0f,
                GLfloat edge = 0.0f);
    // clang-format on

    void useLight( const LightingUniVarBindings& lighting ) const override;

    void setFlash( glm::vec3 pos, glm::vec3 dir );
};

#endif  // _INCLUDED_SPOTLIGHT_H