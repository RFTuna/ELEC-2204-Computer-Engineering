#include "processor.hpp"

#include "output.hpp"

#include <iostream>

Processor::Processor() {
    currentState = State::FETCH;
}

ReturnCode Processor::cycle() {

    currentState = (State)(currentState + 1);

    Output::Debug("Cycle: ");
    Output::DebugLine(currentState);

    return currentState == MEMORY_ACCESS ? ReturnCode::EXIT : ReturnCode::CONTINUE;
}

Processor::~Processor() {
    
}