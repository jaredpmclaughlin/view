#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include <tclap/CmdLine.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace view {

void handle_args(int, char **);

class Solid { // failing to come up with a good name for this class
/* The plan is for this to be the parent class of those  that translates whatever the input is
 * in to something usable for OpenGL. So, there should be another class per
 * input format. 
 */

    private:
    std::vector<GLfloat>* vertex_buffer_data; // try to be like OpenGL and not move data around

    public: 
    Solid();
    int getBufferSize();
    int getVerticeCount();
    std::vector<GLfloat>* getBuffer();

};


class Window {
/*  The plan is for this to be the wrapper around OpenGL and any
*    other platform dependent UI calls that need to be made.
*/

private:
    GLFWwindow* OGLwindow;
    std::vector<GLfloat> *vertex_buffer_data;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    void drawLines(int);
    void GLBuffer();

public:
    int init();
    void loop(Solid &);
 //   void drawLines(int); // This is to reduce calls.
 //   void GLBuffer();
    void setBuffer(std::vector<GLfloat> *);
    int getBufferSize();
    int getVerticeCount();
 

};
}

namespace {

    const std::string err = "error :";
    const std::string GLFW_init_fail_msg = "Failed to initialize GLFW";
    const std::string GLFW_window_fail_msg = "Failed to open GLFW window.";
    const std::string GLEW_init_fail_msg = "Failed to initialize GLEW";
    const std::string MainWindowTitle = "View";
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

    view::Solid object;
    view::Window mainWindow;

    if ( mainWindow.init() == EXIT_FAILURE ) return EXIT_FAILURE;

    mainWindow.loop(object);

    std::cout<<"Success."<<std::endl;
    return EXIT_SUCCESS;
}

view::Solid::Solid(){
    vertex_buffer_data = new std::vector<GLfloat>;
    *vertex_buffer_data = {
        -.50f, -.50f, 0.0f,
            .50f, -.50f, 0.0f,
            0.0f,  .50f, 0.0f,
            -.50f, -.50f, 0.0f,
    };
 
}

int view::Solid::getBufferSize(){
    return this->vertex_buffer_data->size()*sizeof(GLfloat);
}

int view::Solid::getVerticeCount(){
    return this->vertex_buffer_data->size()/3; // 3D space
}

std::vector<GLfloat>* view::Solid::getBuffer(){
    return this->vertex_buffer_data;
}

void view::Window::setBuffer(std::vector<GLfloat>* vertex_buffer){
    this->vertex_buffer_data = vertex_buffer;
}

int view::Window::getBufferSize(){
    return this->vertex_buffer_data->size()*sizeof(GLfloat);
}

int view::Window::getVerticeCount(){
    return this->vertex_buffer_data->size()/3; // 3D space
}
void view::Window::GLBuffer(){

    // the '1' is for how many arrays to generate
    glGenVertexArrays(1, &(this->VertexArrayID)); // how many and where to put them
    glBindVertexArray(this->VertexArrayID); // returns GL_INVALID_OPERATION if you do something wrong
    // 1:1 VAO / VBO
    glGenBuffers(1, &(this->vertexbuffer)); // how many and where
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(this->vertex_buffer_data->size()), this->vertex_buffer_data->data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // uses currently bound VAO
    //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size, 3D space, xyz
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

}

void view::Window::drawLines(int vertices){
        glDrawArrays(GL_LINE_STRIP, 0, vertices); 
        glDisableVertexAttribArray(0);
}

void view::Window::loop(Solid &object) {

    glClearColor(0.5, 0.5, 0.5, 0.5);
    glClearDepth(1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->setBuffer(object.getBuffer());
    this->GLBuffer();
    this->drawLines(this->getVerticeCount()); // outside loop because we only intend to load one thing

    do {

        // Swap buffers
        glfwSwapBuffers(OGLwindow); //only fires off when the rendering is finished? i.e. doesn't block?
        glfwPollEvents(); // if there are a lot of events, this could block for a while
        //glfwWaitEvents may work better here

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(OGLwindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(OGLwindow) == 0 );
    return;
}

int view::Window::init() {
// Initialise GLFW
    glewExperimental = GL_TRUE; // Needed for core profile
    if( !glfwInit() )
    {
        std::cerr<<GLFW_init_fail_msg<<std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

// Open a window and create its OpenGL context
//    OGLwindow = glfwCreateWindow( 1024, 768, MainWindowTitle, NULL, NULL);
    OGLwindow = glfwCreateWindow( 1024, 768, "View", NULL, NULL);

    if( OGLwindow == NULL ) {
        std::cerr<<GLFW_window_fail_msg<<std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(OGLwindow); // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr<<GLEW_init_fail_msg<<std::endl;
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

