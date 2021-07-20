#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>

int main(int argc, char** argv)
{
    // tclap doesn't work right without exception handling
    try
    {
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

    catch(TCLAP::ArgException &e) // catch TCLAP exceptions
    {
        std::cerr<< "error: "<<e.error() <<" for arg "<<e.argId() << std::endl;
    }

    std::cout<<"Success."<<std::endl;
}
