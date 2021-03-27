#pragma once

#include <map>
#include <string>

class Registers
{
private:
static std::map<std::string, unsigned int> names;
static unsigned int data[32];

public:
unsigned int get(std::string name);
void set(std::string name, unsigned int value);

static unsigned int Number(std::string name);

};