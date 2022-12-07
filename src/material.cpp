#include "material.h"

Material::Material( GLfloat intensity, GLfloat shine ) : specularIntensity{ intensity }, shininess{ shine } {
}

void Material::useMaterial( GLuint specularIntensityLocation, GLuint shininessLocation ) {
    glUniform1f( specularIntensityLocation, specularIntensity );
    glUniform1f( shininessLocation, shininess );
}

Material::~Material() {
}