#pragma once

#include "memory.hpp"
#include "registers.hpp"
#include "instructions.hpp"

#include <vector>

#define STATE_COUNT 5

enum State {
    FETCH,
    DECODE,
    EXECUTE,
    MEMORY_ACCESS,
    WRITE_BACK,
};

enum ReturnCode {
    CONTINUE,
    EXIT,
};

class Processor {
private:

State currentState;
unsigned int programCounter;
Memory memory;
Registers registers;

unsigned int instructionRegister;

std::string instructionMnemonic;

unsigned int rsValue;
unsigned int rtValue;
unsigned int rdValue;
unsigned int shamt;
unsigned int immediate;
unsigned int address;

unsigned int result;
unsigned int target;

bool exit = false;

void Fetch();
void Decode();
void Execute();
void Write_Back();
void Memory_Access();

unsigned int signExtImm();
unsigned int zeroExtImm();
unsigned int branchAddr();
unsigned int jumpAddr();

public:

Processor();

void program(std::vector<Instruction *> instructions);

ReturnCode cycle();

~Processor();
};