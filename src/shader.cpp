#include "shader.h"

#include <string.h>

namespace {

    size_t getFileSize( FILE** fp, const char* filePath ) {
        *fp = fopen( filePath, "r" );
        if ( *fp == NULL ) {
            fprintf( stderr, "%s does not exist!\n", filePath );
            exit( 1 );
        }
        int count = 0;
        int c = 0;

        while ( ( c = fgetc( *fp ) ) != EOF ) {
            ++count;
        }
        rewind( *fp );
        return ++count;
    }

    void getShaderCode( char* destination, size_t buffer_size, FILE** fp ) {
        fread( destination, buffer_size, 1, *fp );
        destination[buffer_size - 1] = '\0';
        fclose( *fp );
        *fp = NULL;
    }

    std::string getShaderFromFile( const char* filepath ) {
        FILE* fp;
        size_t buffer_size = getFileSize( &fp, filepath );
        char shaderCode[buffer_size];
        getShaderCode( shaderCode, buffer_size, &fp );
        std::string str( shaderCode );
        return str;
    }
}  // namespace

Shader::Shader()
    : shaderID{ 0 },
      uniformProjection{ -1 },
      uniformModel{ -1 },
      uniformView{ -1 },
      uniformSpecularIntensity{ -1 },
      uniformShininess{ -1 },
      uniformPointLightCount{ 0 } {
    uniformPointLight.reserve( MAX_POINT_LIGHTS );
}

void Shader::createFromString( const char* vertexCode, const char* fragmentCode ) {
    compileShader( vertexCode, fragmentCode );
}

void Shader::compileShader( const char* vertexCode, const char* fragmentCode ) {
    shaderID = glCreateProgram();

    if ( !shaderID ) {
        printf( "Failed to create shader\n" );
        return;
    }

    addShader( shaderID, vertexCode, GL_VERTEX_SHADER );
    addShader( shaderID, fragmentCode, GL_FRAGMENT_SHADER );

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram( shaderID );
    glGetProgramiv( shaderID, GL_LINK_STATUS, &result );
    if ( !result ) {
        glGetProgramInfoLog( shaderID, sizeof( eLog ), NULL, eLog );
        printf( "Error linking program: '%s'\n", eLog );
        return;
    }

    glValidateProgram( shaderID );
    glGetProgramiv( shaderID, GL_VALIDATE_STATUS, &result );
    if ( !result ) {
        glGetProgramInfoLog( shaderID, sizeof( eLog ), NULL, eLog );
        printf( "Error validating program: '%s'\n", eLog );
        return;
    }

    uniformModel = glGetUniformLocation( shaderID, "model" );
    uniformProjection = glGetUniformLocation( shaderID, "projection" );
    uniformView = glGetUniformLocation( shaderID, "view" );
    uniformDirectionalLight.ambientColourLocation =
        glGetUniformLocation( shaderID, "directionalLight.base.colour" );
    uniformDirectionalLight.ambientIntensityLocation =
        glGetUniformLocation( shaderID, "directionalLight.base.ambientIntensity" );
    uniformDirectionalLight.diffuseIntensityLocation =
        glGetUniformLocation( shaderID, "directionalLight.base.diffuseIntensity" );
    uniformDirectionalLight.directionLocation =
        glGetUniformLocation( shaderID, "directionalLight.direction" );
    uniformSpecularIntensity =
        glGetUniformLocation( shaderID, "material.specularIntensity" );
    uniformShininess = glGetUniformLocation( shaderID, "material.shininess" );
    uniformEyePosition = glGetUniformLocation( shaderID, "eyePosition" );
    uniformPointLightCount = glGetUniformLocation( shaderID, "pointLightCount" );

    for ( size_t i = 0; i < MAX_POINT_LIGHTS; i++ ) {
        char locBuff[100] = { '\0' };

        snprintf( locBuff, sizeof( locBuff ), "pointLights[%zu].base.colour", i );
        uniformPointLight[i].ambientColourLocation =
            glGetUniformLocation( shaderID, locBuff );

        snprintf( locBuff, sizeof( locBuff ), "pointLights[%zu].base.ambientIntensity",
                  i );
        uniformPointLight[i].ambientIntensityLocation =
            glGetUniformLocation( shaderID, locBuff );

        snprintf( locBuff, sizeof( locBuff ), "pointLights[%zu].base.diffuseIntensity",
                  i );
        uniformPointLight[i].diffuseIntensityLocation =
            glGetUniformLocation( shaderID, locBuff );

        snprintf( locBuff, sizeof( locBuff ), "pointLights[%zu].position", i );
        uniformPointLight[i].positionLocation = glGetUniformLocation( shaderID, locBuff );

        snprintf( locBuff, sizeof( locBuff ), "pointLights[%zu].constant", i );
        uniformPointLight[i].constantLocation = glGetUniformLocation( shaderID, locBuff );

        snprintf( locBuff, sizeof( locBuff ), "pointLights[%zu].linear", i );
        uniformPointLight[i].linearLocation = glGetUniformLocation( shaderID, locBuff );

        snprintf( locBuff, sizeof( locBuff ), "pointLights[%zu].exponent", i );
        uniformPointLight[i].exponentLocation = glGetUniformLocation( shaderID, locBuff );
    }
}

void Shader::addShader( GLuint theProgram, const char* shaderCode, GLenum shaderType ) {
    GLuint theShader = glCreateShader( shaderType );

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen( shaderCode );

    glShaderSource( theShader, 1, theCode, codeLength );
    glCompileShader( theShader );

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv( theShader, GL_COMPILE_STATUS, &result );
    if ( !result ) {
        glGetShaderInfoLog( theShader, 1024, NULL, eLog );
        fprintf( stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog );
        return;
    }

    glAttachShader( theProgram, theShader );
    glDeleteShader( theShader );
}

void Shader::useShader() {
    glUseProgram( shaderID );
}

void Shader::clearShader() {
    if ( shaderID != 0 ) {
        glDeleteProgram( shaderID );
        shaderID = 0;
    }
    uniformModel = -1;
    uniformProjection = -1;
    uniformView = -1;
    uniformEyePosition = -1;
    uniformSpecularIntensity = -1;
    uniformShininess = -1;
}

GLint Shader::getProjectionLocation() const {
    return uniformProjection;
}

GLint Shader::getModelLocation() const {
    return uniformModel;
}

GLint Shader::getViewLocation() const {
    return uniformView;
}

GLint Shader::getSpecularIntensityLocation() const {
    return uniformSpecularIntensity;
}

GLint Shader::getShininessLocation() const {
    return uniformShininess;
}

GLint Shader::getEyePositionLocation() const {
    return uniformEyePosition;
}

void Shader::getShadersFromFiles( const char* vertexPath, const char* fragmentPath ) {
    std::string vertexString = getShaderFromFile( vertexPath );
    std::string fragmentString = getShaderFromFile( fragmentPath );
    createFromString( vertexString.c_str(), fragmentString.c_str() );
}

void Shader::setDirectionalLight( DirectionalLight& dLight ) const {

    dLight.useLight(
        { .ambientIntensityLocation = uniformDirectionalLight.ambientIntensityLocation,
          .ambientColourLocation = uniformDirectionalLight.ambientColourLocation,
          .diffuseIntensityLocation = uniformDirectionalLight.diffuseIntensityLocation,
          .directionLocation = uniformDirectionalLight.directionLocation } );
}

void Shader::setPointLights( std::vector<PointLight>& pLight ) const {
    int lightCount = pLight.size();
    if ( lightCount > MAX_POINT_LIGHTS )
        lightCount = MAX_POINT_LIGHTS;

    glUniform1i( uniformPointLightCount, lightCount );

    for ( size_t i = 0; i < lightCount; i++ ) {
        pLight[i].useLight(
            { .ambientIntensityLocation = uniformPointLight[i].ambientIntensityLocation,
              .ambientColourLocation = uniformPointLight[i].ambientColourLocation,
              .diffuseIntensityLocation = uniformPointLight[i].diffuseIntensityLocation,
              .positionLocation = uniformPointLight[i].positionLocation,
              .constantLocation = uniformPointLight[i].constantLocation,
              .linearLocation = uniformPointLight[i].linearLocation,
              .exponentLocation = uniformPointLight[i].exponentLocation } );
    }
}

Shader::~Shader() {
    clearShader();
}
