#ifndef _INCLUDED_MATERIAL_H
#define _INCLUDED_MATERIAL_H

#include <GL/glew.h>

class Material {
   private:
    // How bright
    GLfloat specularIntensity;
    // How concentrated into one point
    GLfloat shininess;

   public:
    Material( GLfloat intensity = 0.0f, GLfloat shine = 0.0f );
    ~Material();

    void useMaterial( GLuint specularIntensityLocation, GLuint shininessLocation );
};

#endif  // _INCLUDED_MATERIAL_H
