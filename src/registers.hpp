#pragma once

#include <map>
#include <string>

class Registers
{
private:
static std::map<std::string, unsigned int> registers;

public:
unsigned int get(std::string name);
void set(std::string name, unsigned int value);
};