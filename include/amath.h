#ifndef __AMATH_H__
#define __AMATH_H__

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <random>

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <GLFW/glfw3.h>

#include <GL/gl3w.h>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

#define BUFFER_OFFSET( offset ) ((GLvoid*) (offset))

#endif // __AMATH_H__
