//
// Created by David on 4/11/2016.
//

#include "GLController.h"

GLScene *GLController::_model;
GLController *GLController::_controller;

void GLController::keyHandler(unsigned char key, int mousex, int mousey) {
    if(key=='q'||key=='Q') exit(0); //Exit the program

    if(_model) {
        // and r resets the view:
        if (key =='r' || key == 'R') {
            _model->setPosX(0);
            _model->setPosY(0);
            _model->setThetaX(0);
            _model->setThetaY(0);
            _model->setLastX(0);
            _model->setLastY(0);
            _model->resetDistance();
            _model->resetResolution();
            glutPostRedisplay();
        }

        if (key == 'z' || key == 'Z') {
            _model->decrementDistance();
            glutPostRedisplay();
        }

        if (key == 'x' || key == 'X') {
            _model->incrementDistance();
            glutPostRedisplay();
        }

        if(key == '>') {
            _model->increaseResolution();
            glutPostRedisplay();
        }

        if(key == '<') {
            _model->decreaseResolution();
            glutPostRedisplay();
        }

        if(key == 's') {
            _model->toggleCheckerboard();
            glutPostRedisplay();
        }

        if(key == 't') {
            _model->increaseCheckerboardResolution();
            glutPostRedisplay();
        }

        if(key == 'g') {
            _model->decreaseCheckerboardResolution();
            glutPostRedisplay();
        }

        if(key == 'p') {
            _model->screenShot(1);
        }
    }

}

GLController::GLController(GLOptions options) : _options(options){
    _controller = this;
}

void GLController::setModel(GLScene *model) {
    this->_model = model;
}

void GLController::mouseClickHandler(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && _model) {
        _model->setLastY(y);
        _model->setLastX(x);
    }
}

void GLController::mouseHandler(int x, int y) {

    int amntX = (x - _model->getLastZ());
    int amntY = (y - _model->getLastY());

    if (amntX != 0 && _model) {
        float result = _model->getThetaX() + amntX;
        if (result > 360.0 ) result -= 360.0;
        if (result < 0.0 ) result += 360.0;

        _model->setThetaZ(result);
        _model->setLastZ(x);

        glutPostRedisplay();
    }

    if(amntY != 0 && _model) {
        float theta = _model->getThetaY();
        if(theta < 90.0) theta += 360;

        float result = theta + amntY;
        if (result > 445.0 ) result = 445.0;
        if (result < 275.0 ) result = 275;

        if(result > 360) result -= 360;

        _model->setThetaY(result);
        _model->setLastY(y);

        glutPostRedisplay();
    }
}
