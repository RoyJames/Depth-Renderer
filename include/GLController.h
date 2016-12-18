//
// Created by David on 4/11/2016.
//

#ifndef GLKEYBOARD_H
#define GLKEYBOARD_H

#include <stdlib.h>
#include "GLScene.h"
#include "GLOptions.h"

class GLController {
private:
    static GLScene *_model;
    static GLController *_controller;
    GLOptions _options;
public:
    GLController(GLOptions options);
    void setModel(GLScene *model);
    static void keyHandler(unsigned char key, int mousex, int mousey);
    static void mouseClickHandler(int button, int state, int x, int y);
    static void mouseHandler(int x, int y);
};

#endif //GLKEYBOARD_H
