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
    const char* _filename;
    const char*  _output_dir;
    char* _jpegfilename;
    const char** _argv;
    int _argc;
    int _numCloudsToRender;
    bool _windowed;

    int _windowWidth, _windowHeight;

    GLOptions();
    GLOptions(const GLOptions& options);
    GLOptions(int argc, const char** argv);
};


#endif //GLRENDEROPTIONS_H
