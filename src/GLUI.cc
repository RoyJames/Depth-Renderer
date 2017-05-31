//
// Created by David on 4/10/2016.
//

#include <chrono>
#include "GLUI.h"

GLUI* GLUI::_ui;
GLScene *GLUI::_model;
GLController *GLUI::_kb;
GLFWwindow *GLUI::_window;

GLUI::GLUI(GLOptions options): _options(options)
{
    _ui = this;
}

void GLUI::setupKeyboardFunction() {
    glfwSetKeyCallback(_window, _kb->keyHandler);
}

void GLUI::setupMouseFunction() {
    glfwSetCursorPosCallback(_window, _kb->mouseHandler);
    glfwSetMouseButtonCallback(_window, _kb->mouseClickHandler);
}

void GLUI::init() {

    _model->load();

    // set the background color (white)
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void GLUI::startUI() {

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_VISIBLE, _options._windowed ? GLFW_TRUE : GLFW_FALSE);

    _window = glfwCreateWindow(_options._windowWidth, _options._windowHeight, "Depth Renderer", NULL, NULL);

    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(_window);
    if (gl3wInit()) {
        printf("failed to initialize OpenGL\n");
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(0); //Turn Vsync off to get maximum rendering speed

    setupKeyboardFunction();
    setupMouseFunction();

    init();

    auto start = std::chrono::high_resolution_clock::now();

    while (_model->hasMoreSnapshots() && !glfwWindowShouldClose(_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _model->update();

        _model->screenShot();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_seconds = finish-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(finish);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

    glfwDestroyWindow(_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void GLUI::setModel(GLScene *model) {
    _model = model;
}

void GLUI::setController(GLController *kb) {
    _kb = kb;
}
