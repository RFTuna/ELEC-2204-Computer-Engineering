#pragma once

#include "memory.hpp"
#include "registers.hpp"
#include "instructions.hpp"

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

unsigned int rsValue;
unsigned int rtValue;
unsigned int rdValue;

void Fetch();
void Decode();
void Execute();
void Write_Back();
void Memory_Access();

public:

Processor();

ReturnCode cycle();

~Processor();
};