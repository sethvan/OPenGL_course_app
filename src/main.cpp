#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "camera.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "oglwindow.h"
#include "shader.h"
#include "texture.h"

// A mesh is a collection of vertices, faces and edges that define the shape of a 3d object
std::vector<Mesh> meshList;
std::vector<Shader> shaderList;

// static float toRadians( const float degrees ) {
//     return degrees / 57.29578f;
// }

// I believe this takes the normal of each face that a vertex is a part of and it looks like mathematically
// the average between all the faces that a vertex is a component of is calculated by adding them altogether.
// That average is what is set for each vertex. The second for loop then normalizes that calculated average.
void calcAverageNormals( std::vector<GLuint>& indices, std::vector<GLfloat>& vertices, unsigned int vLength,
                         unsigned int normalOffset ) {
    for ( size_t i = 0; i < indices.size(); i += 3 ) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1( vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1],
                      vertices[in1 + 2] - vertices[in0 + 2] );
        glm::vec3 v2( vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1],
                      vertices[in2 + 2] - vertices[in0 + 2] );

        // Calculates normal by cross product to get perpendicular angle to two lines of triangle
        glm::vec3 normal = glm::cross( v1, v2 );
        normal = glm::normalize( normal );

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;
        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for ( size_t i = 0; i < vertices.size() / vLength; i++ ) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec( vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2] );
        vec = glm::normalize( vec );
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

// Delta time measures elapsed time to know how much to adjust the graphics so as to balance the graphics'
// output in spite of inconsistent system speed, see https://en.wikipedia.org/wiki/Delta_timing .
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Creates Mesh objects, indices are indices of vertices and their order to be drawn.
void CreateObjects() {

    // clang-format off
    std::vector<GLuint> indices = { 
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    
    std::vector<GLfloat> vertices = {  
        // X     Y     Z       U     V        nX    nY    nZ
        -1.0f, -1.0f, -0.6f,   0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 1.0f,   0.5f, 0.0f,    0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,   1.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,   0.5f, 1.0f,    0.0f, 0.0f, 0.0f
    };
    // clang-format on

    calcAverageNormals( indices, vertices, 8, 5 );

    meshList.emplace_back( Mesh() );
    meshList.emplace_back( Mesh() );
    meshList[0].createMesh( vertices.data(), indices.data(), vertices.size(), indices.size() );
    meshList[1].createMesh( vertices.data(), indices.data(), vertices.size(), indices.size() );
}

// Each Shader object here actually contains both a vertex and fragment shader
void createShaders() {
    shaderList.emplace_back();
    shaderList[0].getShadersFromFiles( "res/shaders/vertex1.shader", "res/shaders/fragment1.shader" );
}

int main() {

    OGLWindow mainWindow( 1366, 768 );
    mainWindow.initialize();

    CreateObjects();
    createShaders();

    // per experimenting I suspect uniforms must always be initialized to 0 before getting their location
    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0,
           uniformAmbientIntensity = 0, uniformAmbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0,
           uniformSpecularIntensity = 0, uniformShininess = 0;

    uniformModel = shaderList[0].getModelLocation();
    uniformProjection = shaderList[0].getProjectionLocation();
    uniformView = shaderList[0].getViewLocation();
    uniformAmbientColour = shaderList[0].getAmbientColourLocation();
    uniformAmbientIntensity = shaderList[0].getAmbientIntensityLocation();
    uniformDirection = shaderList[0].getDirectionLocation();
    uniformDiffuseIntensity = shaderList[0].getDiffuseIntensityLocation();
    uniformEyePosition = shaderList[0].getEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
    uniformShininess = shaderList[0].getShininessLocation();

    Camera camera( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), -90.0f, 0.0f, 2.5f, 0.2f );

    Texture brickTexture( "res/textures/brick.png" );
    brickTexture.loadTexture();
    Texture dirtTexture( "res/textures/dirt.png" );
    dirtTexture.loadTexture();

    Material shinyMaterial( 1.0f, 32 );
    Material dullMaterial( 0.3f, 4 );

    Light mainLight( 1.0f, 1.0f, 1.0f, 0.2f, 2.0f, -1.0f, -2.0f, 0.3f );

    // says he only needs to make this once and not recalculate it
    // therefore not in while loop
    glm::mat4 projection =
        glm::perspective( 45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f );

    // Loop until window closed
    while ( !mainWindow.getShouldClose() ) {

        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl( mainWindow.getKeys(), deltaTime );
        camera.mouseControl( mainWindow.getXChange(), mainWindow.getYChange() );

        // Clear window
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        shaderList[0].useShader();

        mainLight.useLight( uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection );

        glUniformMatrix4fv( uniformProjection, 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( uniformView, 1, GL_FALSE, glm::value_ptr( camera.calculateViewMatrix() ) );
        glUniform3f( uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y,
                     camera.getCameraPosition().z );

        glm::mat4 model( 1.0f );

        model = glm::translate( model, glm::vec3( 0.0f, 0.0f, -2.5f ) );
        // model = glm::scale( model, glm::vec3( 0.4f, 0.4f, 1.0f ) );
        glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );

        brickTexture.useTexture();
        shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
        meshList[0].renderMesh();

        model = glm::mat4( 1.0f );
        model = glm::translate( model, glm::vec3( 0.0f, 2.5f, -2.5f ) );
        // model = glm::scale( model, glm::vec3( 0.4f, 0.4f, 1.0f ) );
        glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );

        dirtTexture.useTexture();
        dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
        meshList[1].renderMesh();

        glUseProgram( 0 );

        mainWindow.swapBuffers();
    }

    return 0;
}