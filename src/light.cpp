#include "light.h"

Light::Light( GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity )
    : colour{ red, green, blue }, ambientIntensity{ aIntensity } {
}

void Light::useLight( GLfloat ambientIntensityLocation, GLfloat ambientColourLocation ) {
    // glUniform — Specify the value of a uniform variable for the current program object
    // I am guessing .x .y and .z because coming from gl::vec3 and is a uniform as that
    // is how it is done in shaders.
    glUniform3f( ambientColourLocation, colour.x, colour.y, colour.z );
    glUniform1f( ambientIntensityLocation, ambientIntensity );
}

Light::~Light() {
}