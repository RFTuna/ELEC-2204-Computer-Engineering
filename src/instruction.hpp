#pragma once

enum InstructionFormat
{
    R,
    I,
    J,
};

class Instruction {

protected:
    unsigned int m_opcode : 6;

public:
    virtual InstructionFormat format() = 0;

    virtual unsigned int bits() = 0;
};

class InstructionR : Instruction {
private:
    unsigned int m_rs : 5;
    unsigned int m_rt : 5;
    unsigned int m_rd : 5;
    unsigned int m_shamt : 5;
    unsigned int m_funct : 6;

public:

    InstructionR(unsigned int opcode, unsigned int funct);

    void set_rs(unsigned int rs);
    void set_rt(unsigned int rt);
    void set_rd(unsigned int rd);
    void set_shamt(unsigned int shamt);

    unsigned int get_rs();
    unsigned int get_rt();
    unsigned int get_rd();
    unsigned int get_shamt();

    InstructionFormat format() override;

    unsigned int bits() override;
};

class InstructionI : Instruction {
private:
    unsigned int m_rs : 5;
    unsigned int m_rt : 5;
    unsigned int m_immediate : 16;

public:

    InstructionI(unsigned int opcode);

    void set_rs(unsigned int rs);
    void set_rt(unsigned int rt);
    void set_immediate(unsigned int immediate);

    unsigned int get_rs();
    unsigned int get_rt();
    unsigned int get_immediate();

    InstructionFormat format() override;

    unsigned int bits() override;
};

class InstructionJ : Instruction {
private:
    unsigned int m_address : 26;

public:

    InstructionJ(unsigned int opcode);

    void set_address(unsigned int address);

    unsigned int get_address();

    InstructionFormat format() override;

    unsigned int bits() override;
};