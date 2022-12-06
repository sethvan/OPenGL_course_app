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
#include "mesh.h"
#include "oglwindow.h"
#include "shader.h"
#include "texture.h"

// A mesh is a collection of vertices, faces and edges that define the shape of a 3d object
std::vector<Mesh> meshList;
std::vector<Shader> shaderList;

static float toRadians( const float degrees ) {
    return degrees / 57.29578f;
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
        // X     Y     Z       U     V 
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,    
         0.0f, -1.0f, 1.0f,   0.5f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,   0.5f, 1.0f 
    };
    // clang-format on

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

    OGLWindow mainWindow;
    mainWindow.initialize();

    CreateObjects();
    createShaders();

    Camera camera( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), -90.0f, 0.0f, 2.5f, 0.2f );

    Texture brickTexture( "res/textures/brick.png" );
    brickTexture.loadTexture();
    Texture dirtTexture( "res/textures/dirt.png" );
    dirtTexture.loadTexture();

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

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
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectionLocation();
        uniformView = shaderList[0].getViewLocation();

        glm::mat4 model( 1.0f );

        model = glm::translate( model, glm::vec3( 0.0f, 0.0f, -2.5f ) );
        model = glm::scale( model, glm::vec3( 0.4f, 0.4f, 1.0f ) );
        glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
        glUniformMatrix4fv( uniformProjection, 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( uniformView, 1, GL_FALSE, glm::value_ptr( camera.calculateViewMatrix() ) );
        brickTexture.useTexture();
        meshList[0].renderMesh();

        model = glm::mat4( 1.0f );
        model = glm::translate( model, glm::vec3( 0.0f, 1.0f, -2.5f ) );
        model = glm::scale( model, glm::vec3( 0.4f, 0.4f, 1.0f ) );
        glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
        dirtTexture.useTexture();
        meshList[1].renderMesh();

        glUseProgram( 0 );

        mainWindow.swapBuffers();
    }

    return 0;
}