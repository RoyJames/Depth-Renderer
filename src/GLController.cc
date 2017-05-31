//
// Created by David on 4/11/2016.
//

#include "GLController.h"

GLScene *GLController::_model;
GLController *GLController::_controller;
bool ispressed = false;

void GLController::keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        if(key==GLFW_KEY_Q || key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GLFW_TRUE); //exit(0); //Exit the program

        if(_model) {
            // and r resets the view:
            if (key == GLFW_KEY_R) {
                _model->setPosX(0);
                _model->setPosY(0);
                _model->setThetaX(0);
                _model->setThetaY(0);
                _model->setLastX(0);
                _model->setLastY(0);
                _model->resetDistance();
            }

            if (key == GLFW_KEY_Z) {
                _model->decrementDistance();
            }

            if (key == GLFW_KEY_X) {
                _model->incrementDistance();
            }

            if(key == GLFW_KEY_P) {
                _model->screenShot();
            }
        }
    }
}

GLController::GLController(GLOptions options) : _options(options){
    _controller = this;
}

void GLController::setModel(GLScene *model) {
    this->_model = model;
}

void GLController::mouseClickHandler(GLFWwindow* window, int button, int action, int mods) {
    ispressed = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && _model;
}

void GLController::mouseHandler(GLFWwindow* window, double x, double y) {

    if(ispressed) {

        int amntX = (int)(x - _model->getLastX());
        int amntY = (int)(y - _model->getLastY());

        if (amntX != 0 && _model) {
            float result = _model->getThetaX() * 180.0f/M_PI + amntX;
            if (result > 360.0 ) result -= 360.0;
            if (result < 0.0 ) result += 360.0;

            result = result * M_PI/180;

            _model->setThetaX(result);
            _model->setLastX(x);
        }

        if(amntY != 0 && _model) {
            float theta = _model->getThetaY() * 180.0f/M_PI;
            if(theta < 90.0) theta += 360;

            float result = theta + amntY;
            if (result > 445.0 ) result = 445.0;
            if (result < 275.0 ) result = 275;

            if(result > 360) result -= 360;

            result = result * M_PI/180.0f;

            _model->setThetaY(result);
            _model->setLastY(y);
        }
    }

}
