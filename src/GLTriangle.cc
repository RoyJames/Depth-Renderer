#include "GLTriangle.h"

GLTriangle::GLTriangle() {
	_x1 = vec3();
	_x2 = vec3();
	_x3 = vec3();

	calculateNormal();
}
GLTriangle::GLTriangle(vec3 x1, vec3 x2, vec3 x3) {
	_x1 = x1;
	_x2 = x2;
	_x3 = x3;

	calculateNormal();
}

void GLTriangle::calculateNormal() {
	vec3 V = _x2 - _x1;
	vec3 W = _x3 - _x1;
	_normal = normalize(cross(V, W));
	_normal[3] = 0;
}

ostream &operator<<(ostream& os, GLTriangle &t) {
	os << "GLTriangle{" << endl;
	os << "  p1 " << t._x1 << endl;
	os << "  p2 " << t._x2 << endl;
	os << "  p3 " << t._x3 << endl;
	os << "  n  " << t._normal << endl;
	os << "}" << endl;
	return os;
}

vec3 &GLTriangle::operator[](int idx) {
	switch(idx) {
		case 0: return _x1;
		case 1: return _x2;
		case 2: return _x3;
		default: throw std::invalid_argument("Must be 0-2");
	}
}
