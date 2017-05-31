//
// Created by David on 4/10/2016.
//

#include "GLOptions.h"
#include "boost/program_options.hpp" 
#include "boost/filesystem.hpp"

#define WINDOW_HEIGHT 424
#define WINDOW_WIDTH 512

namespace po = boost::program_options;

GLOptions::GLOptions() {
    _argv = 0;
    _argc = 0;
    _windowHeight = WINDOW_HEIGHT;
    _windowWidth = WINDOW_WIDTH;
    _filename = (char*) "";
    _output_dir = (char*) "";
    _numCloudsToRender= 0;
    _verbose = false;
    _windowed = false;
}

GLOptions::GLOptions(const GLOptions& options) {
    _argv = options._argv;
    _argc = options._argc;
    _windowHeight = options._windowHeight;
    _windowWidth = options._windowWidth;
    _filename = strdup(options._filename);
    _output_dir  = strdup(options._output_dir);
    _numCloudsToRender= options._numCloudsToRender;
    _verbose = options._verbose;
    _windowed = options._windowed;
}

GLOptions::GLOptions(int argc, const char **argv) {

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
      ("-window", "Show the rendered image in a window")
      ("mfilename", po::value<std::string>(),"filename of the obj to render partial pointclouds from.") 
      ("output_dir", po::value<std::string>()->default_value("./clouds/"),"Where to save output")
      ("-n", po::value<int>()->default_value(500), "Number of Clouds to Render")
      ("verbose", po::value<bool>()->default_value(true), "Verbose"); 

    po::variables_map vm; 
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm); // can throw 

    /** --help option 
       */ 
    if ( vm.count("help")  || vm.count("-h")) 
    { 
        show_usage(argv[0]); 
        exit(EXIT_SUCCESS);
    } 

    std::cout << "Running with following: \n";
    for (const auto& it : vm) {
        std::cout << "\t" << it.first.c_str() << " ";
        auto& value = it.second.value();
        if (auto v = boost::any_cast<int>(&value))
            std::cout << *v <<  "\n";
        else if (auto v = boost::any_cast<std::string>(&value))
            std::cout << *v<<  "\n";
        else if (auto v = boost::any_cast<bool>(&value))
            std::cout << *v<<  "\n";
        else
            std::cout << "unknown"<<  "\n";
    }

    _filename = vm["mfilename"].as<std::string>().c_str();
    _output_dir = vm["output_dir"].as<std::string>().c_str();
    _numCloudsToRender = vm["-n"].as<int>();
    _windowed = false;
    _verbose = false;
    _windowHeight = WINDOW_HEIGHT;
    _windowWidth = WINDOW_WIDTH;
    _argv = argv;
    _argc = argc;

    // if (strlen(_output_dir) == 0)
    // {
    //     _output_dir = "./clouds";
    // }

    boost::filesystem::path dir(_output_dir);
    if(boost::filesystem::create_directory(dir))
    {
        std::cout << "creating new data directory: " << _output_dir << std::endl;
    }
    else{
        std::cout << "overwriting data in: " << _output_dir << std::endl;
    }
}

void GLOptions::show_usage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)> scenefilename\n"
    << "Options:\n"
    << "\t-h,--help\t\t\tShow this help message\n"
    << "\t-v,--verbose\t\t\tShow verbose messages\n"
    << "\t-h\t\t\tPrint help messages\n"
    << "\t-window\t\t\tShow the rendered image in a window\n"
    << "\tmfilename\t\t\tfilename of the obj to render partial pointclouds from\n"
    << "\toutput_dir\t\t\tWhere to save output\n"
    << "\t-n\t\t\tNumber of Clouds to Render\n"
    << std::endl;
}
