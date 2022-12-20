#include "pointlight.h"

// clang-format off
PointLight::PointLight( GLfloat red, GLfloat green, GLfloat blue,
                        GLfloat aIntensity, GLfloat dIntensity, 
                        GLfloat xPos, GLfloat yPos, GLfloat zPos,
                        GLfloat con, GLfloat lin, GLfloat exp ) 
                        : Light(red, green, blue, aIntensity, dIntensity), 
                        position{xPos, yPos, zPos}, 
                        constant{con}, linear{lin}, exponent{exp} {}
// clang-format on

void PointLight::useLight( const LightingUniVarBindings& lighting ) const {
    glUniform3f( lighting.ambientColourLocation, colour.x, colour.y, colour.z );
    glUniform1f( lighting.ambientIntensityLocation, ambientIntensity );
    glUniform1f( lighting.diffuseIntensityLocation, diffuseIntensity );
    glUniform3f( lighting.positionLocation, position.x, position.y, position.z );
    glUniform1f( lighting.constantLocation, constant );
    glUniform1f( lighting.linearLocation, linear );
    glUniform1f( lighting.exponentLocation, exponent );
}