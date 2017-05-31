//
// Created by David on 4/10/2016.
//

#ifndef GL_RENDER_UI_H
#define GL_RENDER_UI_H

#include "GL/gl3w.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
//#include <GL/glew.h>
//#include <GL/freeglut.h>
//#include <GL/freeglut_ext.h>
#include <SOIL/SOIL.h>
#endif

#include <chrono>
#include <vector>
#include <functional>
#include "amath.h"
#include "GLOptions.h"
#include "GLScene.h"
#include "GLShaderLoader.h"
#include "GLController.h"
#include <GLFW/glfw3.h>

#define UI_DISPLAY_MODE GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE

class GLUI {

    void init();
    void setupKeyboardFunction();
    void setupMouseFunction();

    static GLUI *_ui;
    static GLScene *_model;
    static GLController *_kb;
    static int _posx, _posy;
    GLOptions _options;
    static GLFWwindow *_window;

public:
    GLUI(GLOptions options);
    void setModel(GLScene *model);
    void setController(GLController *kb);
    void startUI();
};


#endif //GL_RENDER_UI_H
