//
// Created by David on 4/10/2016.
//

#ifndef INITSHADER_H
#define INITSHADER_H

#include "amath.h"
#include <GL/glew.h>

char* readShaderSource(const char* shaderFile);
GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

#endif //INITSHADER_H
