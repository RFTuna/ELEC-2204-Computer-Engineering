#include "processor.hpp"

#include "output.hpp"

#include <iostream>

Processor::Processor() {
    currentState = State::FETCH;
    programCounter = 0x04000000;
}

ReturnCode Processor::cycle() {

    switch(currentState)
    {
    case FETCH:
    Fetch();
    break;

    case DECODE:
    Decode();
    break;

    case EXECUTE:
    Execute();
    break;

    case WRITE_BACK:
    Write_Back();
    break;

    case MEMORY_ACCESS:
    Memory_Access();
    break;
    }

    currentState = (State)(currentState + 1);

    return currentState == MEMORY_ACCESS ? ReturnCode::EXIT : ReturnCode::CONTINUE;
}

void Processor::Fetch()
{
    instructionRegister = memory.get(programCounter);
}

void Processor::Decode()
{
    programCounter += 4;
}

void Processor::Execute()
{

}

void Processor::Memory_Access()
{
    
}

void Processor::Write_Back()
{

}

Processor::~Processor() {
    
}