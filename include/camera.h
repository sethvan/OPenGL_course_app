#ifndef _INCLUDED_CAMERA_H
#define _INCLUDED_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
  private:
   glm::vec3 position;
   glm::vec3 front;
   glm::vec3 up;
   glm::vec3 right;
   glm::vec3 worldUp;
   glm::vec3 direction;

   GLfloat yaw;    // left and right
   GLfloat pitch;  // up and down

   GLfloat movementSpeed;
   GLfloat turnSpeed;

   void update();

  public:
   Camera() = default;
   Camera( glm::vec3 startPosition, glm::vec3 startWorldUp, GLfloat startYaw, GLfloat startPitch,
           GLfloat startMovementSpeed, GLfloat startTurnSpeed );

   void keyControl( bool* keys, GLfloat deltaTime );
   void mouseControl( GLfloat xChange, GLfloat yChange );
   void scrollControll( GLfloat yoffset, GLfloat deltaTime );

   glm::mat4 calculateViewMatrix();
   glm::vec3 getCameraPosition() const;
   glm::vec3 getCameraDirection();
};

#endif  // _INCLUDED_CAMERA_H