#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace view {

void handle_args(int argc, char** argv){
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

}

int main(int argc, char** argv)
{
   try{ 
    view::handle_args(argc, argv);    
}
    catch(TCLAP::ArgException &e) // catch TCLAP exceptions
    {
        std::cerr<< "error: "<<e.error() <<" for arg "<<e.argId() << std::endl;
    }

    std::cout<<"Success."<<std::endl;
}
