#include "processor.hpp"

#include "output.hpp"

#include "instructions.hpp"

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

    currentState = (State)((currentState + 1) % STATE_COUNT);

    return exit ? EXIT : CONTINUE;
}

void Processor::Fetch()
{
    Output::DebugLine("");
    Output::DebugLine("");
    Output::DebugLine("");
    Output::DebugLine("");
    Output::DebugLine("");
    Output::DebugLine("");
    Output::DebugLine("||||||||||||||||||||||||||||||||||||||");
    Output::DebugLine("FETCH");

    Output::Debug("pc: ");
    Output::DebugLine(programCounter);

    instructionRegister = memory.getWord(programCounter);
}

void Processor::Decode()
{
    programCounter += 4;

    Instruction *instruction = Instructions::decode(instructionRegister);

    instructionMnemonic = instruction->mnemonic();

    Output::DebugLine("");
    Output::DebugLine("DECODE");

    Output::Debug("instruction: ");
    Output::DebugLine(instructionMnemonic);

    Output::DebugLine(instruction->outputString());

    if(instruction->format() == R)
    {
        rsValue = registers.get(((InstructionR *)instruction)->get_rs());
        rtValue = registers.get(((InstructionR *)instruction)->get_rt());
        rdValue = registers.get(((InstructionR *)instruction)->get_rd());
        shamt = ((InstructionR *)instruction)->get_shamt();

        target = ((InstructionR *)instruction)->get_rd();

        Output::Debug("rs: ");
        Output::DebugLine(((InstructionR *)instruction)->get_rs());

        Output::Debug("rt: ");
        Output::DebugLine(((InstructionR *)instruction)->get_rt());

        Output::Debug("rs value: ");
        Output::DebugLine(rsValue);

        Output::Debug("rt value: ");
        Output::DebugLine(rtValue);

        Output::Debug("shamt: ");
        Output::DebugLine(shamt);

        Output::Debug("target: ");
        Output::DebugLine(target);

    }
    else if(instruction->format() == I)
    {
        rsValue = registers.get(((InstructionI *)instruction)->get_rs());
        rtValue = registers.get(((InstructionI *)instruction)->get_rt());
        immediate = ((InstructionI *)instruction)->get_immediate();

        target = ((InstructionI *)instruction)->get_rt();

        Output::Debug("rs: ");
        Output::DebugLine(((InstructionI *)instruction)->get_rs());

        Output::Debug("rs value: ");
        Output::DebugLine(rsValue);

        Output::Debug("immediate: ");
        Output::DebugLine(immediate);

        Output::Debug("target: ");
        Output::DebugLine(target);
    }
    else if(instruction->format() == J)
    {
        address = ((InstructionJ *)instruction)->get_address();

        Output::Debug("address: ");
        Output::DebugLine(address);
    }
}

void Processor::Execute()
{
    Output::DebugLine("");
    Output::DebugLine("EXECUTE");

    if(instructionMnemonic == "addu")
        result = rsValue + rtValue;
    if(instructionMnemonic == "addiu")
        result = rsValue + signExtImm();
    if(instructionMnemonic == "subu")
        result = rsValue - rtValue;
    else if(instructionMnemonic == "and")
        result = rsValue & rtValue;
    else if(instructionMnemonic == "andi")
        result = rsValue & zeroExtImm();
    else if(instructionMnemonic == "or")
        result = rsValue | rtValue;
    else if(instructionMnemonic == "ori")
        result = rsValue | zeroExtImm();
    else if(instructionMnemonic == "nor")
        result = ~(rsValue | rtValue);
    else if (instructionMnemonic == "sll")
        result = rsValue << shamt;
    else if (instructionMnemonic == "srl")
        result = rsValue >> shamt;
    else if (instructionMnemonic == "sra")
        result = ((int)rsValue) >> shamt; //implementation defined, but gcc is arithmetic shift
    else if (instructionMnemonic == "sltu")
        result = rsValue < rtValue ? 1 : 0;
    else if (instructionMnemonic == "sltiu")
        result = rsValue < signExtImm() ? 1 : 0;
    else if (instructionMnemonic == "slt")
        result = ((int)rsValue) < ((int)rtValue) ? 1 : 0;
    else if (instructionMnemonic == "slti")
        result = ((int)rsValue) < ((int)signExtImm()) ? 1 : 0;
    else if(instructionMnemonic == "beq")
        programCounter = rsValue == rtValue ? programCounter + branchAddr() : programCounter;
    else if(instructionMnemonic == "bne")
        programCounter = rsValue != rtValue ? programCounter + branchAddr() : programCounter;
    else if(instructionMnemonic == "jr")
        programCounter = rsValue;
    else if(instructionMnemonic == "j")
        programCounter = jumpAddr();
    else if(instructionMnemonic == "jal")
    {
        programCounter = jumpAddr();
        result = programCounter + 4;
    }
    else if(instructionMnemonic == "lui")
        result = immediate << 16;
}

void Processor::Memory_Access()
{
    Output::DebugLine("");
    Output::DebugLine("MEMORY ACCESS");

    if(instructionMnemonic == "lbu")
        result = memory.getByte(rsValue + signExtImm());
    if(instructionMnemonic == "lhu")
        result = memory.getHalfword(rsValue + signExtImm());
    if(instructionMnemonic == "lw")
        result = memory.getWord(rsValue + signExtImm());
    if(instructionMnemonic == "sb")
        memory.setByte(rsValue + signExtImm(), rtValue & 0x000000FF);
    if(instructionMnemonic == "sh")
        memory.setHalfword(rsValue + signExtImm(), rtValue & 0x0000FFFF);
    if(instructionMnemonic == "sw")
        memory.setWord(rsValue + signExtImm(), rtValue);
}

void Processor::Write_Back()
{
    Output::DebugLine("");
    Output::DebugLine("WRITE BACK");

    if(instructionMnemonic != "sb"
    && instructionMnemonic != "sh"
    && instructionMnemonic != "sw"
    && instructionMnemonic != "j"
    && instructionMnemonic != "jr"
    && instructionMnemonic != "beq"
    && instructionMnemonic != "bne"
    && instructionMnemonic != "syscall")
    {
        Output::Debug("result: ");
        Output::DebugLine(result);

        if(instructionMnemonic == "jal")
        {
            registers.set(31, result);
        }
        else
        {
            registers.set(target, result);
        }
    }

    if(instructionMnemonic == "syscall")
        exit = true;
}

unsigned int Processor::signExtImm()
{
    return (immediate & 0x00008000 ? 0xffff0000 : 0) + immediate; 
}

unsigned int Processor::zeroExtImm()
{
    return immediate;
}

unsigned int Processor::branchAddr()
{
    return (immediate & 0x00008000 ? 0xfffc0000 : 0) + (immediate << 2);
}

unsigned int Processor::jumpAddr()
{
    return (programCounter & 0xF0000000) + (address << 2);
}

void Processor::program(std::vector<Instruction *> instructions)
{
    for(unsigned int i = 0; i < instructions.size(); i++)
    {
        memory.setWord(0x04000000 + i * 4, instructions[i]->bits());
    }
}

Processor::~Processor() {
    
}