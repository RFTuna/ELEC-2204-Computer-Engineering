#pragma once

#include <iostream>
#include <iomanip>

enum OutputMode {
    RELEASE,
    DEBUG
};

class Output {
private:

static bool newLine;

public:

static OutputMode mode;

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
}
};