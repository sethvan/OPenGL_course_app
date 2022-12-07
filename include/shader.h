#ifndef _INCLUDED_SHADER_H
#define _INCLUDED_SHADER_H

#include <GL/glew.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>

class Shader {
   private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformAmbientIntensity, uniformAmbientColour;

    void addShader( GLuint theProgram, const char* shaderCode, GLenum shaderType );
    void compileShader( const char* vertexCode, const char* fragmentCode );

   public:
    Shader();
    ~Shader();

    void createFromString( const char* vertexCode, const char* fragmentCode );
    void getShadersFromFiles( const char* vertexPath, const char* fragmentPath );

    GLuint getProjectionLocation() const;
    GLuint getModelLocation() const;
    GLuint getViewLocation() const;
    GLuint getAmbientIntensityLocation() const;
    GLuint getAmbientColourLocation() const;

    void useShader();
    void clearShader();
};

#endif