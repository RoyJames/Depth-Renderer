//
// Created by David on 4/10/2016.
//

#include "GLOptions.h"
#include "boost/program_options.hpp" 

namespace po = boost::program_options;

GLOptions::GLOptions() {
    _argv = 0;
    _argc = 0;
    _windowHeight = 424;
    _windowWidth = 512;
    _filename = (char*) "";
    _verbose = false;
}

GLOptions::GLOptions(const GLOptions& options) {
    _argv = options._argv;
    _argc = options._argc;
    _windowHeight = options._windowHeight;
    _windowWidth = options._windowWidth;
    _filename = options._filename;
    _verbose = options._verbose;
}

GLOptions::GLOptions(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Too few arguments\n";
        show_usage(argv[0]);
        exit(1);
    }

    po::options_description desc("Options"); 

    po::positional_options_description p;
    p.add("mfilename", -1);

    desc.add_options() 
      ("help", "Print help messages") 
      ("-h", "Print help messages") 
      ("mfilename", po::value<std::string>(),"filename of the obj to render partial pointclouds from.") 
      ("-n", po::value<int>(&_numCloudsToRender)->default_value(100), "Number of Clouds to Render")
      ("height", po::value<int>(&_windowHeight)->default_value(424), "Window Height")
      ("width", po::value<int>(&_windowWidth)->default_value(512), "Window Width")
      ("verbose", po::value<bool>(&_verbose)->default_value(true), "Verbose"); 

    po::variables_map vm; 
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm); // can throw 

    /** --help option 
       */ 
    if ( vm.count("help")  || vm.count("-h")) 
    { 
        show_usage(argv[0]); 
        exit(EXIT_SUCCESS);
    } 

    _filename = vm["mfilename"].as<std::string>().c_str();

    _argv = argv;
    _argc = argc;

    _verbose = false;

    _argv = 0;
    _argc = 0;
    _windowHeight = 424;
    _windowWidth = 512;
    _verbose = false;

}

void GLOptions::show_usage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)> scenefilename\n"
    << "Options:\n"
    << "\t-h,--help\t\t\tShow this help message\n"
    << "\t-v,--verbose\t\t\tShow verbose messages\n"
    << std::endl;
}
