#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GLVector.h"
#include <iostream>
#include <stdexcept>

using namespace std;

class GLTriangle {
private:
	vec3 _x1, _x2, _x3;
	void calculateNormal();

public:
	GLTriangle();
	GLTriangle(vec3 x1, vec3 x2, vec3 x3);
	vec4 _normal;

	vec3 &operator[](int idx);
	friend ostream &operator<<(ostream& os, GLTriangle &t);
};


#endif //TRIANGLE_H