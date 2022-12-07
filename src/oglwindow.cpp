#include "oglwindow.h"

OGLWindow::OGLWindow( GLint windowWidth, GLint windowHeight )
    : width{ windowWidth }, height{ windowHeight }, xChange{ 0.0f }, yChange{ 0.0f } {
    for ( size_t i = 0; i < 1024; i++ ) {
        keys[i] = 0;
    }
}

int OGLWindow::initialize() {
    // Initialise GLFW
    if ( !glfwInit() ) {

        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    // Core Profileew
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    // Allow Forward Compatbility
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    // Create the window
    mainWindow = glfwCreateWindow( width, height, "Test Window", NULL, NULL );
    if ( !mainWindow ) {

        glfwTerminate();
        return 1;
    }

    // Get Buffer Size information
    glfwGetFramebufferSize( mainWindow, &bufferWidth, &bufferHeight );

    // Set context for GLEW to use
    glfwMakeContextCurrent( mainWindow );

    // Handle key and mouse input
    createCallbacks();

    // Disable cursor
    glfwSetInputMode( mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK ) {
        printf( "GLEW initialisation failed!" );
        glfwDestroyWindow( mainWindow );
        glfwTerminate();
        return 1;
    }

    // GL_DEPTH_TEST: Once enabled, OpenGL automatically stores fragments their z-values in the depth buffer if they
    // passed the depth test and discards fragments if they failed the depth test accordingly. If you have depth testing
    // enabled you should also clear the depth buffer before each frame using GL_DEPTH_BUFFER_BIT; otherwise you're
    // stuck with the depth values from last frame:
    glEnable( GL_DEPTH_TEST );

    // Setup Viewport size
    glViewport( 0, 0, bufferWidth, bufferHeight );

    // Necessary to know what user owns the window
    // Because the callbacks need to be static methods in order for glfw to call them this function stores the specific
    // instance of `this` so it can later be retrieved by glfwGetWindowUserPointer() inside the callback to handle the
    // correct and desired data.
    glfwSetWindowUserPointer( mainWindow, this );

    return 0;
}

// This is a callback we created to handle the output from glfwSetKeyCallback() which are the input parameters
// to this method.
void OGLWindow::handleKeys( GLFWwindow* window, int key, int code, int action, int mode ) {
    OGLWindow* theWindow = static_cast<OGLWindow*>( glfwGetWindowUserPointer( window ) );
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
        glfwSetWindowShouldClose( window, GL_TRUE );
    }

    if ( key >= 0 && key <= 1024 ) {
        if ( action == GLFW_PRESS ) {
            theWindow->keys[key] = true;
        }
        else if ( action == GLFW_RELEASE ) {
            theWindow->keys[key] = false;
        }
    }
}

void OGLWindow::createCallbacks() {
    glfwSetKeyCallback( mainWindow, handleKeys );
    glfwSetCursorPosCallback( mainWindow, handleMouse );
}

// he said this wasn't really necessary, I didn;t understand its purpose
GLfloat OGLWindow::getXChange() {
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}
// he said this wasn't really necessary, I didn;t understand its purpose
GLfloat OGLWindow::getYChange() {
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

// See handleKeys() above.
void OGLWindow::handleMouse( GLFWwindow* window, double xPos, double yPos ) {
    OGLWindow* theWindow = static_cast<OGLWindow*>( glfwGetWindowUserPointer( window ) );
    if ( theWindow->mouseFirstMoved ) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }
    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

OGLWindow::~OGLWindow() {
    glfwDestroyWindow( mainWindow );
    glfwTerminate();
}