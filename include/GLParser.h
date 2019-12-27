#ifndef PARSE_H
#define PARSE_H

class GLParser;

#include "GLScene.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tiny_obj_loader.h>

using namespace std;

class GLParser {
private:
	void parseObj(const char* file, GLScene &scene);
public:
    GLParser();
  	void parse(const char *file, GLScene &scene);
  	void parseviews(const char *file, GLScene &scene);
};

#endif
