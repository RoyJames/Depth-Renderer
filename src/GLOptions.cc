//
// Created by David on 4/10/2016.
//

#include "GLOptions.h"

GLOptions::GLOptions() {
    _argv = 0;
    _argc = 0;
    _windowHeight = 424;
    _windowWidth = 512;
    _filename = (char*) "";
    _verbose = false;
    _jpegfilename = (char*) "kitten.jpg";
}

GLOptions::GLOptions(const GLOptions& options) {
    _argv = options._argv;
    _argc = options._argc;
    _windowHeight = options._windowHeight;
    _windowWidth = options._windowWidth;
    _filename = options._filename;
    _verbose = options._verbose;
    _jpegfilename = options._jpegfilename;
}

GLOptions::GLOptions(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Too few arguments\n";
        show_usage(argv[0]);
        exit(1);
    }

    _filename = (char*) "";
    _verbose = false;

    _argv = argv;
    _argc = argc;

    /* TODO: Read in window width/height from command line */
    _windowHeight = 424;
    _windowWidth = 512;

    bool filenameset = false;
    bool jpegfilenameset = false;
    for(int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            exit(0);
        } else if((arg == "-v") || (arg == "--verbose")) {
            _verbose = true;
        } else {
            if(!filenameset) {
                _filename = argv[i];
                filenameset = true;
            } else if(!jpegfilenameset) {
                jpegfilenameset = true;
                _jpegfilename = argv[i];
            }
            else {
                std::cerr << "Unknown parameter \"" << argv[i] << "\"\n";
                show_usage(argv[0]);
                exit(1);
            }
        }
    }
}

void GLOptions::show_usage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)> scenefilename\n"
    << "Options:\n"
    << "\t-h,--help\t\t\tShow this help message\n"
    << "\t-v,--verbose\t\t\tShow verbose messages\n"
    << std::endl;
}
