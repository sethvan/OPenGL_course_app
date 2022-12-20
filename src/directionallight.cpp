#include "directionallight.h"

#include <iostream>

DirectionalLight::DirectionalLight( GLfloat red, GLfloat green, GLfloat blue,
                                    GLfloat aIntensity, GLfloat dIntensity, GLfloat xDir,
                                    GLfloat yDir, GLfloat zDir )
    : Light( red, green, blue, aIntensity, dIntensity ), direction{ xDir, yDir, zDir } {
}

void DirectionalLight::useLight( const LightingUniVarBindings& lighting ) const {
    // glUniform — Specify the value of a uniform variable for the current program object
    // I am guessing .x .y and .z because coming from gl::vec3 and is a uniform as that
    // is how it is done in shaders.
    glUniform3f( lighting.ambientColourLocation, colour.x, colour.y, colour.z );
    glUniform1f( lighting.ambientIntensityLocation, ambientIntensity );
    glUniform1f( lighting.diffuseIntensityLocation, diffuseIntensity );
    glUniform3f( lighting.directionLocation, direction.x, direction.y, direction.z );
}
