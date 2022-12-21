#include "camera.h"

#include <stdio.h>

Camera::Camera( glm::vec3 startPosition, glm::vec3 startWorldUp, GLfloat startYaw,
                GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed )
    : position{ startPosition },
      front{ glm::vec3( 0.0f, 0.0f, -1.0f ) },
      worldUp{ startWorldUp },
      yaw{ startYaw },
      pitch{ startPitch },
      movementSpeed{ startMovementSpeed },
      turnSpeed{ startTurnSpeed } {

    direction = glm::normalize( position - front );
    update();
}

void Camera::update() {

    direction.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
    direction.y = sin( glm::radians( pitch ) );
    direction.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
    front = glm::normalize(
        direction );  // normalize means we just want direction not magnitude of vector

    right = glm::normalize( glm::cross( front, worldUp ) );
    up = glm::normalize( glm::cross( right, front ) );
}

void Camera::keyControl( bool* keys, GLfloat deltaTime ) {

    GLfloat velocity = movementSpeed * deltaTime;

    if ( keys[GLFW_KEY_W] ) {
        position += front * velocity;
    }
    if ( keys[GLFW_KEY_S] ) {
        position -= front * velocity;
    }
    if ( keys[GLFW_KEY_A] ) {
        position -= right * velocity;
    }
    if ( keys[GLFW_KEY_D] ) {
        position += right * velocity;
    }
}

glm::mat4 Camera::calculateViewMatrix() {
    return glm::lookAt( position, position + front, up );
}

void Camera::mouseControl( GLfloat xChange, GLfloat yChange ) {
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if ( pitch > 89.0f ) {
        pitch = 89.0f;
    }
    if ( pitch < -89.0f ) {
        pitch = -89.0f;
    }
    update();
}

glm::vec3 Camera::getCameraPosition() const {
    return position;
}

glm::vec3 Camera::getCameraDirection() {
    return front;
}