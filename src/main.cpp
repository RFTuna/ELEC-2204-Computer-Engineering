#include <iostream>

#include "parser.hpp"
#include "processor.hpp"
#include "output.hpp"

Processor processor = Processor();

int main(int argc, char* argv[])
{
    std::string filename = "";

    bool log = false;

    for(int i = 0; i < argc; i++)
    {
        if(std::string(argv[i]) == "debug")
            Output::mode = DEBUG;

        if(std::string(argv[i]) == "-f")
            filename = std::string(argv[i + 1]);

        if(std::string(argv[i]) == "log")
            log = true;
    }

        if(log)
        {
            std::string logFile(filename);
            Output::SetLogFile(logFile.append(".log"));
        }

    std::vector<Instruction *> code = Parser::parse(filename.append(".mips"));

    processor.program(code);

    ReturnCode returnCode;
    
    do {
        returnCode = processor.cycle();
    }
    while (returnCode != ReturnCode::EXIT);

    Output::PrintLine("Terminated Successfully");
}