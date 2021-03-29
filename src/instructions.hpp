#pragma once

#include "instruction.hpp"

#include <map>
#include <string>

struct InstructionPrototype
{
    InstructionFormat format;
    unsigned int arguments[3];
};

class Instructions
{
private:

//static std::map<std::string, Instruction> instructions;

static std::map<std::string, InstructionPrototype> instructions;

public:
static Instruction *decode(unsigned int bits);

static Instruction *create(std::string mnemonic);
};