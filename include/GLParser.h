#ifndef PARSE_H
#define PARSE_H

class GLParser;

#include <GL/gl3w.h>

#include "GLTriangle.h"
#include "GLScene.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class GLParser {
private:
	void parseObj(const char* file, GLScene &scene);
public:
    GLParser();
  	void parse(const char *file, GLScene &scene);
};

#endif
