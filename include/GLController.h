//
// Created by David on 4/11/2016.
//

#ifndef GLKEYBOARD_H
#define GLKEYBOARD_H

#include <stdlib.h>
#include "GLScene.h"
#include "GLOptions.h"
#include <GLFW/glfw3.h>

class GLController {
private:
    static GLScene *_model;
    static GLController *_controller;
    GLOptions _options;
public:
    GLController(GLOptions options);
    void setModel(GLScene *model);
    static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseClickHandler(GLFWwindow* window, int button, int action, int mods);
    static void mouseHandler(GLFWwindow* window, double x, double y);
};

#endif //GLKEYBOARD_H
