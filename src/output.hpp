#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>

enum OutputMode {
    RELEASE,
    DEBUG
};

class Output {
private:

static bool newLine;

static std::ofstream log;

static std::string logFile;

static bool shouldLog;

public:

static OutputMode mode;

static void SetLogFile(std::string file);

template <typename T>
static void Print(T message)
{
    if(newLine)
        std::cout << std::setfill(' ') << std::setw(20) << std::left << "[RELEASE]";
    std::cout << message;
    newLine = false;
}

template <typename T>
static void PrintLine(T message)
{
    if(newLine)
        std::cout << std::setfill(' ') << std::setw(20) << std::left << "[RELEASE]";
    std::cout << message << std::endl;
    newLine = true;
}

template <typename T>
static void Debug(T message)
{
    if(mode != RELEASE)
    {
        if(newLine)
            std::cout << std::setfill(' ') << std::setw(20) << std::left << "[DEBUG]";
        std::cout << message;
        newLine = false;
    }

    if(shouldLog)
    {
        log << message;
    }
}

template <typename T>
static void DebugLine(T message)
{
    if(mode != RELEASE)
    {
        if(newLine)
            std::cout << std::setfill(' ') << std::setw(20) << std::left << "[DEBUG]";
        std::cout << message << std::endl;
        newLine = true;
    }

    if(shouldLog)
    {
        log << message << std::endl;
    }
}
};