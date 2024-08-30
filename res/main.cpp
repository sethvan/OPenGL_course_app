#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "camera.h"
#include "directionallight.h"
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "oglwindow.h"
#include "pointlight.h"
#include "shader.h"
#include "spotlight.h"
#include "texture.h"

// rando stuff
// Define a random number generator engine
std::random_device rd;
std::default_random_engine eng( rd() );

// Define the range of the random number
float lower_bound = 200.0f;
float upper_bound = 300.0f;

// Define the distribution that produces numbers within the range
std::uniform_real_distribution<> distr( lower_bound, upper_bound );

// A mesh is a collection of vertices, faces and edges that define the shape of a 3d
// object
std::vector<Mesh> meshList;
std::vector<Shader> shaderList;

static float toRadians( const float degrees ) {
   return degrees / 57.29578f;
}

// I believe this takes the normal of each face that a vertex is a part of and it looks
// like mathematically the average between all the faces that a vertex is a component of
// is calculated by adding them altogether. That average is what is set for each vertex.
// The second for loop then normalizes that calculated average.
void calcAverageNormals( std::vector<GLuint>& indices, std::vector<GLfloat>& vertices, unsigned int vLength,
                         unsigned int normalOffset ) {
   for ( size_t i = 0; i < indices.size(); i += 3 ) {
      unsigned int in0 = indices[ i ] * vLength;
      unsigned int in1 = indices[ i + 1 ] * vLength;
      unsigned int in2 = indices[ i + 2 ] * vLength;
      // clang-format off
        // Calculate normal by cross product to get perpendicular angle to two lines of triangle
        glm::vec3 v1( vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2] );
        glm::vec3 v2( vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2] );
        glm::vec3 normal = glm::cross( v1, v2 );
        normal = glm::normalize( normal );

        // I beieve this adds up all the corresponding normals to a vertex
        // as in all the faces of the trangles that a vertex is a part of.
        // Then in the following loop below normalizing that sum gives the 
        // normalized average of those norms. Tis is the preferred way to 
        // handle objects with many edges so as to transmit the appearance of smoothnessa.
        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;

    }

    for ( size_t i = 0; i < vertices.size() / vLength; i++ ) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec( vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2] );
        vec = glm::normalize( vec );
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
     //clang-format on
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
        //                     Texture          Normal
        // X     Y     Z       U     V        nX    nY    nZ
        -1.0f, -1.0f, -0.6f,   0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 1.0f,    0.5f, 0.0f,    0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,   1.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,    0.5f, 1.0f,    0.0f, 0.0f, 0.0f
    };

    std::vector<GLuint> floorIndices = {
		0, 2, 1,
		1, 2, 3
	};

	std::vector<GLfloat> floorVertices = {
		-100.0f, 0.0f, -100.0f,	0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		100.0f,  0.0f, -100.0f,	100.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-100.0f, 0.0f, 100.0f,	   0.0f,  100.0f,	0.0f, -1.0f, 0.0f,
		100.0f,  0.0f, 100.0f,    100.0f, 100.0f,	0.0f, -1.0f, 0.0f
	};

   // clang-format on

   calcAverageNormals( indices, vertices, 8, 5 );

   meshList.emplace_back( Mesh() );
   meshList.emplace_back( Mesh() );
   meshList.emplace_back( Mesh() );

   meshList[ 0 ].createMesh( vertices.data(), indices.data(), vertices.size(), indices.size() );
   meshList[ 1 ].createMesh( vertices.data(), indices.data(), vertices.size(), indices.size() );
   meshList[ 2 ].createMesh( floorVertices.data(), floorIndices.data(), floorVertices.size(), floorIndices.size() );
}

// Each Shader object here actually contains both a vertex and fragment shader
void createShaders() {
   shaderList.emplace_back();
   shaderList[ 0 ].getShadersFromFiles( "res/shaders/vertex1.shader", "res/shaders/fragment1.shader" );
}

int main() {
   bool direction = true;
   float triOffset = 0.0f;
   float triMaxOffset = 0.7f;
   float triIncrement = 0.0005f;

   int count = 0;
   int limit = distr( eng );
   float increment = 0.3f;

   float curAngle = 0.0f;
   float curAlt = 0.0f;

   OGLWindow mainWindow( 1920, 1080 );
   mainWindow.initialize();

   CreateObjects();
   createShaders();

   // per experimenting I suspect uniforms must always be initialized to 0 before getting
   // their location
   GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0,
          uniformSpecularIntensity = 0, uniformShininess = 0;

   uniformModel = shaderList[ 0 ].getModelLocation();
   uniformProjection = shaderList[ 0 ].getProjectionLocation();
   uniformView = shaderList[ 0 ].getViewLocation();
   uniformEyePosition = shaderList[ 0 ].getEyePositionLocation();
   uniformSpecularIntensity = shaderList[ 0 ].getSpecularIntensityLocation();
   uniformShininess = shaderList[ 0 ].getShininessLocation();

   Camera camera( glm::vec3( 0.0f, 0.25f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), -60.0f, 0.0f, 21.5f, 0.06f );

   Texture brickTexture( "res/textures/brick.png" );
   brickTexture.loadTextureA();
   Texture dirtTexture( "res/textures/dirt.png" );
   dirtTexture.loadTextureA();
   Texture plainTexture( "res/textures/plain.png" );
   plainTexture.loadTextureA();

   Material shinyMaterial( 1.0f, 32 );
   Material dullMaterial( 0.3f, 4 );

   Model xwing;
   xwing.loadModel( "res/models/x-wing.obj" );
   Model vaderTie;
   vaderTie.loadModel( "res/models/Star Wars vader tie.obj" );

   // clang-format off
    DirectionalLight mainLight( 1.0f, 1.0f, 1.0f,   // colour
                                0.8f, 0.1f,         // aIntensity, dIntensity
                                0.0f, 0.0f, -1.0f );// direction

    std::vector<PointLight> pointLights;
    pointLights.emplace_back( 0.0f, 0.0f, 1.0f,
								      0.0f, 1.0f,
							      	0.0f, 0.0f, 0.0f,
							      	0.3f, 0.2f, 0.1f );   // constant, linear, exponent <- attenuation

    pointLights.emplace_back( 0.0f, 1.0f, 0.0f,
                              0.0f, 0.1f,
                              -4.0f, 2.0f, 0.0f,
                              0.3f, 0.1f, 0.1f );

    std::vector<SpotLight> spotLights;
    spotLights.emplace_back( 1.0f, 1.0f, 1.0f,     // colour
                             0.0f, 2.0f,           // aIntensity, dIntensity
                             0.0f, 0.0f, 0.0f,     // position
                             0.0f, -1.0f, 0.0f,    // direction
                             1.0f, 0.0f, 0.007f,     // constant, linear, exponent
                             20.0f );              // edge

     spotLights.emplace_back( 1.0f, 1.0f, 1.0f,     // colour
                             0.0f, 1.0f,           // aIntensity, dIntensity
                             0.0f, -1.5f, 0.0f,     // position
                             -100.0f, -1.0f, 0.0f, // direction
                             1.0f, 0.2f, 0.1f,     // constant, linear, exponent 
                             20.0f );              // edge
   // clang-format on

   // says he only needs to make this once and not recalculate it
   // therefore not in while loop
   glm::mat4 projection =
       glm::perspective( 45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f );
   float lastYaw = -60.0f;
   float tilt = 0.0f;
   float factorWeight = 1.0f;
   float sign = 1.0f;
   float lastSign = sign;
   // Loop until window closed
   while ( !mainWindow.getShouldClose() ) {
      if ( direction ) {
         triOffset += triIncrement;
      } else {
         triOffset -= triIncrement;
      }

      if ( abs( triOffset ) >= triMaxOffset ) {
         direction = !direction;
      }

      curAngle -= 0.22f;
      if ( curAngle >= 360 ) {
         curAngle -= 360;
      }

      GLfloat now = glfwGetTime();
      deltaTime = now - lastTime;
      lastTime = now;

      if ( ++count > static_cast<int>( limit ) ) {
         count = 0;
         limit = distr( eng );

         increment *= -1;
         std::cerr << "limit: " << limit << '\n';
      }
      if ( curAlt > 1.5f ) {
         curAlt -= abs( increment * deltaTime );
      } else if ( curAlt < -1.5f ) {
         curAlt += abs( increment * deltaTime );
      } else {
         curAlt += ( increment * deltaTime );
      }

      glfwPollEvents();

      camera.keyControl( mainWindow.getKeys(), deltaTime );
      camera.mouseControl( mainWindow.getXChange(), mainWindow.getYChange() );
      camera.scrollControll( mainWindow.getYOffset(), deltaTime );

      // Clear window
      glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      glm::vec3 lowerLight = camera.getCameraPosition();
      lowerLight.y += 1.3f;

      spotLights[ 0 ].setFlash( lowerLight, camera.getCameraDirection() );

      shaderList[ 0 ].useShader();
      shaderList[ 0 ].setDirectionalLight( mainLight );
      shaderList[ 0 ].setPointLights( pointLights );

      shaderList[ 0 ].setSpotLights( spotLights );

      glUniformMatrix4fv( uniformProjection, 1, GL_FALSE, glm::value_ptr( projection ) );
      glUniformMatrix4fv( uniformView, 1, GL_FALSE, glm::value_ptr( camera.calculateViewMatrix() ) );
      glUniform3f( uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y,
                   camera.getCameraPosition().z );
      glm::vec3 position = camera.getCameraPosition();
      glm::vec3 direction = camera.getCameraDirection();

      glm::mat4 model = glm::mat4( 1.0f );

      model = glm::mat4( 1.0f );
      // model = glm::translate( model, change );
      //

      model = glm::translate( model, glm::vec3( position ) );
      model = glm::scale( model, glm::vec3( 0.006f, 0.006f, 0.006f ) );
      model = glm::rotate( model, -toRadians( camera.yaw ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
      model = glm::rotate( model, toRadians( camera.pitch ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
      model = glm::translate( model, glm::vec3( 390.0f, -45.0f, 10.0f ) );
      float incFactor = 6 * std::min( factorWeight, 1.05f );

      float yawChangeValue = camera.yaw - lastYaw;
      sign = std::signbit( yawChangeValue ) ? -1.0f : 1.0f;
      float rotation = yawChangeValue * incFactor;
      bool yawHasChanged = ( ( abs( rotation ) > 2.5f ) || ( abs( rotation ) > 0.001f ) && ( abs( tilt ) > 0.49f ) );

      printf( "Here!\n" );

      if ( yawHasChanged || abs( tilt ) > 0.49f ) {
         if ( yawHasChanged ) {
            if ( abs( rotation ) > 2.5f ) {
               factorWeight += 0.0005f;
               auto adjustment = std::min( abs( rotation ), 0.5f );
               if ( std::signbit( sign ) == std::signbit( tilt ) ) {
                  tilt += adjustment;
               } else {
                  tilt -= adjustment;
               }
            }
            lastSign = sign;

         } else {
            factorWeight = 1.0f;
            if ( tilt <= 0.5f && tilt >= -0.5f ) {
               tilt = 0.0f;
            } else if ( tilt > 0.0f ) {
               tilt -= 0.5f;
            } else if ( tilt < 0.0f ) {
               tilt += 0.5f;
            }
         }
      }
      model = glm::rotate( model, toRadians( tilt ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
      model = glm::rotate( model, toRadians( 90.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
      model = glm::translate( model, glm::vec3( -1401.5920f, -108.5028f, 1722.1118f ) );

      glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
      shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
      xwing.renderModel();
      lastYaw = camera.yaw;

      model = glm::mat4( 1.0f );
      model = glm::translate( model, glm::vec3( 0.0f, -2.0f, 0.0f ) );
      glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
      dirtTexture.useTexture();
      shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
      meshList[ 2 ].renderMesh();

      model = glm::mat4( 1.0f );

      model = glm::rotate( model, toRadians( curAngle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
      model = glm::translate( model, glm::vec3( 90.0f, curAlt + 0.75f, 0.0f ) );
      model = glm::rotate( model, toRadians( 37.0f - ( curAlt * 20.0f ) ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
      model = glm::scale( model, glm::vec3( 0.006f, 0.006f, 0.006f ) );
      model = glm::translate( model, glm::vec3( -1400.0172f, -108.2528f, 1722.1118f ) );

      glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
      shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
      xwing.renderModel();

      model = glm::mat4( 1.0f );
      model = glm::rotate( model, toRadians( curAngle + 5.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
      model = glm::translate( model, glm::vec3( 90.0f, -curAlt + 1.0f, 0.0f ) );
      model = glm::rotate( model, toRadians( 20.0f + ( curAlt * 20.0f ) ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
      model = glm::rotate( model, toRadians( -25.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
      model = glm::scale( model, glm::vec3( 0.006f, 0.006f, 0.006f ) );
      model = glm::translate( model, glm::vec3( 109.477, -720.436, 320.566 ) );

      glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
      shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
      vaderTie.renderModel();

      // model = glm::mat4( 1.0f );
      // model = glm::translate( model, glm::vec3( 0.8f, .0f, -1.5f ) );
      // model = glm::scale( model, glm::vec3( 0.006f, 0.006f, 0.006f ) );
      // model = glm::rotate( model, toRadians( -30.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
      // model = glm::translate( model, glm::vec3( -position ) );
      // glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
      // shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
      // lordVader.renderModel();

      glUseProgram( 0 );

      mainWindow.swapBuffers();
   }

   return 0;
}