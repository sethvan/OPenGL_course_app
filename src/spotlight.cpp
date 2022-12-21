#include "spotlight.h"

#include <stdio.h>

// clang-format off
SpotLight::SpotLight( GLfloat red, GLfloat green, GLfloat blue, \
                      GLfloat aIntensity, GLfloat dIntensity, 
                      GLfloat xPos, GLfloat yPos, GLfloat zPos,
                      GLfloat xDir, GLfloat yDir, GLfloat zDir, 
                      GLfloat con, GLfloat lin, GLfloat exp,
                      GLfloat edg )
    : PointLight( red, green, blue, 
                  aIntensity, dIntensity,
                  xPos, yPos, zPos,
                  con, lin, exp ),
      direction{ glm::normalize(glm::vec3( xDir, yDir, zDir )) },
      edge{ edg },
      procEdge{cosf(glm::radians(edge))} {
}
// clang-format on

void SpotLight::useLight( const LightingUniVarBindings& lighting ) const {
    glUniform3f( lighting.ambientColourLocation, colour.x, colour.y, colour.z );
    glUniform1f( lighting.ambientIntensityLocation, ambientIntensity );
    glUniform1f( lighting.diffuseIntensityLocation, diffuseIntensity );
    glUniform3f( lighting.directionLocation, direction.x, direction.y, direction.z );
    glUniform3f( lighting.positionLocation, position.x, position.y, position.z );
    glUniform1f( lighting.constantLocation, constant );
    glUniform1f( lighting.linearLocation, linear );
    glUniform1f( lighting.exponentLocation, exponent );
    glUniform1f( lighting.edgeLocation, procEdge );
}

void SpotLight::setFlash( glm::vec3 pos, glm::vec3 dir ) {
    position = pos;
    direction = dir;
}
