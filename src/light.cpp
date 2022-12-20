#include "light.h"

Light::Light( GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
              GLfloat dIntensity )
    : colour{ red, green, blue },
      ambientIntensity{ aIntensity },
      diffuseIntensity{ dIntensity } {
}

void Light::useLight( const LightingUniVarBindings& lighting ) const {
    glUniform3f( lighting.ambientColourLocation, colour.x, colour.y, colour.z );
    glUniform1f( lighting.ambientIntensityLocation, ambientIntensity );
    glUniform1f( lighting.diffuseIntensityLocation, diffuseIntensity );
}