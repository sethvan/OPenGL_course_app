#include "light.h"

Light::Light( GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat dIntensity )
    : colour{ red, green, blue },
      ambientIntensity{ aIntensity },
      direction{ xDir, yDir, zDir },
      diffuseIntensity{ dIntensity } {
}

void Light::useLight( GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
                      GLuint directionLocation ) {
    // glUniform — Specify the value of a uniform variable for the current program object
    // I am guessing .x .y and .z because coming from gl::vec3 and is a uniform as that
    // is how it is done in shaders.
    glUniform3f( ambientColourLocation, colour.x, colour.y, colour.z );
    glUniform1f( ambientIntensityLocation, ambientIntensity );

    glUniform3f( directionLocation, direction.x, direction.y, direction.z );
    glUniform1f( diffuseIntensityLocation, diffuseIntensity );
}

Light::~Light() {
}