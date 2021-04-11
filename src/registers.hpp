#pragma once

#include <map>
#include <string>

class Registers
{
private:
static std::map<std::string, unsigned int> names;
unsigned int data[32];

public:
unsigned int get(unsigned int number);
void set(unsigned int number, unsigned int value);

static unsigned int Number(std::string name);
static std::string Name(unsigned int number);

void Debug();
};