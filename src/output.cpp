#include "output.hpp"

bool Output::newLine = true;

OutputMode Output::mode = RELEASE;

std::ofstream Output::log;

std::string Output::logFile;

bool Output::shouldLog = false;

void Output::SetLogFile(std::string file)
{
    logFile = file;
    log.open(file, std::ofstream::out | std::ofstream::trunc);

    if(log.is_open())
    {
        shouldLog = true;
    }
}