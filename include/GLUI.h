//
// Created by David on 4/10/2016.
//

#ifndef GL_RENDER_UI_H
#define GL_RENDER_UI_H

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <SOIL/SOIL.h>
#endif

#include <vector>
#include <functional>
#include "GLTriangle.h"
#include "amath.h"
#include "GLOptions.h"
#include "GLScene.h"
#include "GLShaderLoader.h"
#include "GLController.h"

#define UI_DISPLAY_MODE GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE

class GLUI {
private:
    static void display();
    static void timerFunc(int value);

    void init();
    void setupDisplayFunction();
    void setupKeyboardFunction();
    void setupMouseFunction();

    static GLUI *_ui;
    static GLScene *_model;
    static GLController *_kb;
    static int _posx, _posy;
    GLOptions _options;

public:
    GLUI(GLOptions options);
    void setModel(GLScene *model);
    void setController(GLController *kb);
    void startUI();
};


#endif //GL_RENDER_UI_H
