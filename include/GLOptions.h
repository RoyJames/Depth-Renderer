//
// Created by David on 4/10/2016.
//

#ifndef GLRENDEROPTIONS_H
#define GLRENDEROPTIONS_H

#include <string>
#include <iostream>

class GLOptions {
private:
    void show_usage(std::string name);
public:
    bool _verbose;
    char* _filename;
    char* _jpegfilename;
    char** _argv;
    int _argc;

    int _windowWidth, _windowHeight;

    GLOptions();
    GLOptions(const GLOptions& options);
    GLOptions(int argc, char** argv);
};


#endif //GLRENDEROPTIONS_H
