#ifndef _INCLUDED_SHADER_H
#define _INCLUDED_SHADER_H

#include <GL/glew.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "directionallight.h"
#include "pointlight.h"

#define MAX_POINT_LIGHTS 3

class Shader {
   private:
    GLuint shaderID;
    GLint uniformProjection, uniformModel, uniformView, uniformEyePosition,
        uniformSpecularIntensity, uniformShininess, uniformPointLightCount;
    std::vector<LightingUniVarBindings> uniformPointLight;
    LightingUniVarBindings uniformDirectionalLight;

    void addShader( GLuint theProgram, const char* shaderCode, GLenum shaderType );
    void compileShader( const char* vertexCode, const char* fragmentCode );

   public:
    Shader();
    ~Shader();

    void createFromString( const char* vertexCode, const char* fragmentCode );
    void getShadersFromFiles( const char* vertexPath, const char* fragmentPath );

    GLint getProjectionLocation() const;
    GLint getModelLocation() const;
    GLint getViewLocation() const;
    GLint getSpecularIntensityLocation() const;
    GLint getShininessLocation() const;
    GLint getEyePositionLocation() const;

    void useShader();
    void clearShader();
    void setDirectionalLight( DirectionalLight& dLight ) const;
    void setPointLights( std::vector<PointLight>& pLight ) const;
};

#endif