#include "instructions.hpp"

#include "output.hpp"

std::map<std::string, InstructionPrototype> Instructions::instructions
{
    //{"add", {R, {0, 0x20}}},
    //{"addi", {I, {0x8}}},
    {"addiu", {I, {0x9}}}, //
    {"addu", {R, {0, 0x21}}}, //

    {"and", {R, {0, 0x24}}}, //
    {"andi", {I, {0xc}}}, //

    {"beq", {I, {0x4}}}, //
    {"bne", {I, {0x5}}}, //

    {"j", {J, {0x2}}}, //
    {"jal", {J, {0x3}}}, //
    {"jr", {R, {0, 0x8}}}, //

    {"lbu", {I, {0x24}}}, //
    {"lhu", {I, {0x25}}}, //
    {"lui", {I, {0xf}}}, //
    {"lw", {I, {0x23}}}, //

    {"nor", {R, {0, 0x27}}}, //
    {"or", {R, {0, 0x25}}}, //
    {"ori", {I, {0xd}}}, //

    {"slt", {R, {0, 0x2a}}}, //
    {"stli", {I, {0xa}}}, //
    {"sltiu", {I, {0xb}}}, //
    {"sltu", {R, {0, 0x2b}}}, //

    {"sll", {R, {0, 0x00}}}, //
    {"srl", {R, {0, 0x02}}}, //

    {"sb", {I, {0x28}}}, //
    {"sh", {I, {0x29}}}, //
    {"sw", {I, {0x2b}}}, //

    //{"sub", {R, {0, 0x22}}},
    {"subu", {R, {0, 0x23}}}, //

    //{"mfc0", {R, {0x10, 0x0}}}, sed for exceptions

    {"sra", {R, {0, 0x3}}}, //

    {"syscall", {R, {0, 0xc}}},
};

std::string Instructions::mnemonic(unsigned int bits)
{
    unsigned int opcode = bits >> 26;

    unsigned int funct = bits & 0x3F;

    std::string mnemonic;

    for(auto const& [key, val] : instructions)
    {
        if(opcode == 0 && val.arguments[0] == 0)
        {
            if(val.arguments[1] == funct)
            {
                mnemonic = key;
                break;
            }
        }
        else if(val.arguments[0] == opcode)
        {
            mnemonic = key;
            break;
        }
    }

    return mnemonic;
}

Instruction *Instructions::decode(unsigned int bits)
{
    Instruction *instruction = create(mnemonic(bits));

    if(instruction->format() == R)
    {
        ((InstructionR *)instruction)->set_rs((bits >> 21) & 0x1F);
        ((InstructionR *)instruction)->set_rt((bits >> 16) & 0x1F);
        ((InstructionR *)instruction)->set_rd((bits >> 11) & 0x1F);
        ((InstructionR *)instruction)->set_shamt((bits >> 6) & 0x1F);
    }
    else if(instruction->format() == I)
    {
        ((InstructionI *)instruction)->set_rs((bits >> 21) & 0x1F);
        ((InstructionI *)instruction)->set_rt((bits >> 16) & 0x1F);
        ((InstructionI *)instruction)->set_immediate(bits & 0xFFFF);
    }
    else if(instruction->format() == J)
    {
        ((InstructionJ *)instruction)->set_address(bits & 0x3FFFFFF);
    }

    return instruction;
}

Instruction *Instructions::create(std::string mnemonic)
{
    InstructionPrototype prototype = instructions[mnemonic];

    switch(prototype.format)
    {
        case R:
            return (Instruction *)(new InstructionR(prototype.arguments[0], prototype.arguments[1]));

        case I:
            return (Instruction *)(new InstructionI(prototype.arguments[0]));

        case J:
            return (Instruction *)(new InstructionJ(prototype.arguments[0]));
    }

    return nullptr;
}