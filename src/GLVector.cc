//
// Created by David on 4/10/2016.
//

#include "GLVector.h"

vec2 vec2::operator/(const GLfloat s) const {
    GLfloat r = GLfloat(1.0) / s;
    return *this * r;
}

vec2 &vec2::operator/=(const GLfloat s) {
    GLfloat r = GLfloat(1.0) / s;
    *this *= r;

    return *this;
}
