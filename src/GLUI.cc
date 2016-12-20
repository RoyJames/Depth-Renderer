//
// Created by David on 4/10/2016.
//

#include "GLUI.h"

#include <stdlib.h>     /* exit, EXIT_FAILURE */

GLUI* GLUI::_ui;
GLScene *GLUI::_model;
GLController *GLUI::_kb;

GLUI::GLUI(GLOptions options):
    _options(options)
{
    _ui = this;
}

void GLUI::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _model->update();
    _model->reload();

    glutSwapBuffers();
}

void GLUI::timerFunc(int value) {
    glutPostRedisplay();
    _model->screenShot(0);
    glutTimerFunc(1, timerFunc, 0);
}

void GLUI::setupDisplayFunction() {
    glutDisplayFunc(display);
}

void GLUI::setupKeyboardFunction() {
    glutKeyboardFunc(_kb->keyHandler);
}

void GLUI::setupMouseFunction() {
    glutMotionFunc(_kb->mouseHandler);
    glutMouseFunc(_kb->mouseClickHandler);
}

void GLUI::init() {

    _model->load();

    _model->loadShaders();

    // set the background color (white)
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

void GLUI::startUI() {
    glutInit(&_options._argc, _options._argv);
    glutInitDisplayMode(UI_DISPLAY_MODE);

    glutInitWindowSize(_options._windowWidth, _options._windowHeight);
    glutCreateWindow("Depth Renderer");

    setupDisplayFunction();
    setupKeyboardFunction();
    setupMouseFunction();

    #ifndef __APPLE__
        glewInit();
    #endif

    init();

    if(_options._windowed) {

        glutTimerFunc(1, timerFunc, 0);
        glutMainLoop();
    } else {
        _model->renderOffScreen();
    }
}

void GLUI::setModel(GLScene *model) {
    _model = model;
}

void GLUI::setController(GLController *kb) {
    _kb = kb;
}
