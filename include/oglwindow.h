#ifndef _INCLUDED_OGLWINDOW_H
#define _INCLUDED_OGLWINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

class OGLWindow {
   private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;

    bool keys[1024];

    static void handleKeys( GLFWwindow* window, int key, int code, int action, int mode );
    static void handleMouse( GLFWwindow* window, double xPos, double yPos );
    void createCallbacks();

   public:
    OGLWindow( GLint windowWidth = 800, GLint windowHeight = 600 );

    int initialize();

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose( mainWindow ); }

    bool* getKeys() { return keys; }

    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { glfwSwapBuffers( mainWindow ); }

    ~OGLWindow();
};

#endif  // _INCLUDED_OGLWINDOW_H