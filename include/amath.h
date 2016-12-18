#ifndef __AMATH_H__
#define __AMATH_H__

#include <cmath>
#include <iostream>
#include <stdio.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);
const GLfloat  DegreesToRadians = M_PI / 180.0;

#endif // __AMATH_H__
