#include "output.hpp"

bool Output::newLine = true;

OutputMode Output::mode = RELEASE;

/*
void Output::Print(const char *message)
{
    if(newLine)
        std::cout << std::setfill(' ') << std::setw(20) << std::left << "[RELEASE]";
    std::cout << message;
    newLine = false;
}

void Output::PrintLine(const char *message)
{
    if(newLine)
        std::cout << std::setfill(' ') << std::setw(20) << std::left << "[RELEASE]";
    std::cout << message << std::endl;
    newLine = true;
}

void Output::Debug(const char *message)
{
    if(mode != RELEASE)
    {
        if(newLine)
            std::cout << std::setfill(' ') << std::setw(20) << std::left << "[DEBUG]";
        std::cout << message;
        newLine = false;
    }
}

void Output::DebugLine(const char *message)
{
    if(mode != RELEASE)
    {
        if(newLine)
            std::cout << std::setfill(' ') << std::setw(20) << std::left << "[DEBUG]";
        std::cout << message << std::endl;
        newLine = true;
    }
}*/