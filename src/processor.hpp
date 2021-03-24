#pragma once

enum State {
    FETCH,
    DECODE,
    EXECUTE,
    WRITE_BACK,
    MEMORY_ACCESS,
};

enum ReturnCode {
    CONTINUE,
    EXIT,
};

class Processor {
private:

State currentState;

public:

Processor();

ReturnCode cycle();

~Processor();
};