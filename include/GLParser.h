#ifndef PARSE_H
#define PARSE_H

class GLParser;

#include "GLTriangle.h"
#include "GLScene.h"
#include "GLBezierSurface.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class GLParser {
private:
	void parseBezier(const char* file, GLScene &scene);
	void parseObj(const char* file, GLScene &scene);
public:
    GLParser();
  	void parse(const char *file, GLScene &scene);
};

#endif
