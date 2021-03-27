#include "instructions.hpp"

std::map<std::string, InstructionPrototype> Instructions::instructions
{
    {"add", {R, {0, 0x20}}},
    {"addi", {I, {0x8}}},
    {"addiu", {I, {0x9}}},
    {"addu", {R, {0, 0x21}}},

    {"and", {R, {0, 0x24}}},
    {"andi", {I, {0xc}}},

    {"beq", {I, {0x4}}},
    {"bne", {I, {0x5}}},

    {"j", {J, {0x2}}},
    {"jal", {J, {0x3}}},
    {"jr", {R, {0, 0x8}}},

    {"lbu", {I, {0x24}}},
    {"lhu", {I, {0x25}}},
    {"lui", {I, {0xf}}},
    {"lw", {I, {0x23}}},

    {"nor", {R, {0, 0x27}}},
    {"or", {R, {0, 0x25}}},
    {"ori", {I, {0xd}}},

    {"slt", {R, {0, 0x2a}}},
    {"stli", {I, {0xa}}},
    {"sltiu", {I, {0xb}}},
    {"sltu", {R, {0, 0x2b}}},

    {"sll", {R, {0, 0x00}}},
    {"srl", {R, {0, 0x02}}},

    {"sb", {I, {0x28}}},
    {"sh", {I, {0x29}}},
    {"sw", {I, {0x2b}}},

    {"sub", {R, {0, 0x22}}},
    {"subu", {R, {0, 0x23}}}, 

    //{"mfc0", {R, {0x10, 0x0}}}, sed for exceptions

    {"sra", {R, {0, 0x3}}},
};

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

/*
std::map<std::string, InstructionPrototype> Instructions::instructions
{
    {"add", {R, {0, 0x20}}},
    {"addi", {I, {0x8}}},
    {"addiu", {I, {0x9}}},
    {"addu", {R, {0, 0x21}}},
    {"and", {R, {0, 0x24}}},
    {"andi", {I, {0xc}}},
    {"beq", {I, {0x4}}},
    {"bne", {I, {0x5}}},
    {"j", {J, {0x2}}},
    {"jal", {J, {0x3}}},
    {"jr", {R, {0, 0x8}}},
    {"lbu", {I, {0x24}}},
    {"lhu", {I, {0x25}}},
    {"lui", {I, {0xf}}},
    {"lw", {I, {0x23}}},
    {"nor", {R, {0, 0x27}}},
    {"or", {R, {0, 0x25}}},
    {"ori", {I, {0xd}}},
    {"slt", {R, {0, 0x2a}}},
    {"stli", {I, {0xa}}},
    {"sltiu", {I, {0xb}}},
    {"sltu", {R, {0, 0x2b}}},
    {"sll", {R, {0, 0x00}}},
    {"srl", {R, {0, 0x02}}},
    {"sb", {I, {0x28}}},
    {"sh", {I, {0x29}}},
    {"sw", {I, {0x2b}}},
    {"sub", {R, {0, 0x22}}},
    {"subu", {R, {0, 0x23}}},
    {"bclt", {FI, {0x11, 0x8, 0x1}}},
    {"bclf", {FI, {0x11, 0x8, 0x0}}},
    {"add.s", {FR, {0x11, 0x10, 0x0}}},
    {"add.d", {FR, {0x11, 0x11, 0x0}}},
    {"c.eq.s", {FR, {0x11, 0x10, 0x32}}},
    {"c.lt.s", {FR, {0x11, 0x10, 0x3c}}},
    {"c.le.s", {FR, {0x11, 0x10, 0x3e}}},
    {"c.eq.d", {FR, {0x11, 0x11, 0x32}}},
    {"c.lt.d", {FR, {0x11, 0x11, 0x3c}}},
    {"c.le.d", {FR, {0x11, 0x11, 0x3e}}},
    {"sub.s", {FR, {0x11, 0x10, 0x1}}},
    {"sub.d", {FR, {0x11, 0x11, 0x1}}},
    {"lwcl", {I, {0x31}}},
    {"ldcl", {I, {0x35}}},
    {"mfhi", {R, {0, 0x10}}},
    {"mflo", {R, {0, 0x12}}},
    {"mfc0", {R, {0x10, 0x0}}},
    {"sra", {R, {0, 0x3}}},
    {"swcl", {I, {0x39}}},
    {"sdcl", {I, {0x3d}}}
};
*/