#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace view {

void handle_args(int, char **);
class window {
private:
    GLFWwindow* OGLwindow; // (In the accompanying source code, this variable is global for simplicity)

public:
    int init();
    void loop();
};
}

namespace {
const std::string err = "error :";
const std::string GLFW_fail = "Failed to initialize GLFW";

}

int main(int argc, char** argv)
{
    // command line argument processing
    try {
        view::handle_args(argc, argv);
    }
    catch(TCLAP::ArgException &e) // catch TCLAP exceptions
    {
        std::cerr<< "error: "<<e.error() <<" for arg "<<e.argId() << std::endl;
    }

    view::window mainWindow;

    if ( mainWindow.init() == EXIT_FAILURE ) return EXIT_FAILURE;

    mainWindow.loop();

    std::cout<<"Success."<<std::endl;
    return EXIT_SUCCESS;
}

void view::window::loop() {
    do {
        //glClear( GL_COLOR_BUFFER_BIT );

// draw here

        // Swap buffers
        glfwSwapBuffers(OGLwindow);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(OGLwindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(OGLwindow) == 0 );
    return;
}

int view::window::init() {
// Initialise GLFW
    glewExperimental = GL_TRUE; // Needed for core profile
    if( !glfwInit() )
    {
        std::cerr<<GLFW_fail<<std::endl;
        //std::cerr<<"Failed to initialize GLFW"<<std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

// Open a window and create its OpenGL context
    //GLFWwindow* OGLwindow; // (In the accompanying source code, this variable is global for simplicity)
    OGLwindow = glfwCreateWindow( 1024, 768, "view", NULL, NULL);
    if( OGLwindow == NULL ) {
        std::cerr<<"Failed to open GLFW window."<<std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(OGLwindow); // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr<<"Failed to initialize GLEW"<<std::endl;
        return EXIT_FAILURE;
    }

// Ensure we can capture the escape key being pressed below
    glfwSetInputMode(OGLwindow, GLFW_STICKY_KEYS, GL_TRUE);
    return EXIT_SUCCESS;
}

void view::handle_args(int argc, char** argv) {
    // tclap doesn't work right without exception handling
    // describe the program and seperator
    TCLAP::CmdLine cmd("Description message.", ' ', "0.1");
    // define an argument, the boolean is for required / not
    TCLAP::ValueArg<std::string> dummyArg("d","dummy","A dummy arg.",false,"homer","string");
    // add the argument
    cmd.add(dummyArg);

    // Here's a switch, this version adds itself, so you don't need the above call
    TCLAP::SwitchArg dummySwitch("s", "switch", "Switch the switch.", cmd, false);

    // parse args
    cmd.parse(argc,argv);

    // get values
    std::string dummy = dummyArg.getValue();
    bool switchValue = dummySwitch.getValue();
}

