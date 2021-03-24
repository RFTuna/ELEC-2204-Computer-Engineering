#pragma once

/*

enum OPCODE {
    OPCODE_FUNCT1 = 0b000000,
    OPCODE_J = 0b000010,
    OPCODE_JAL = 0b000011,
    OPCODE_BEQ = 0b000100,
    OPCODE_BNE = 0b000101,
    OPCODE_BLEZ = 0b000110,
    OPCODE_BGTZ = 0b000111,
    OPCODE_ADDI = 0b001000,
    OPCODE_ADDIU = 0b001001,
    OPCODE_SLTI = 0b001010,
    OPCODE_SLTIU = 0b001011,
    OPCODE_ANDI = 0b001100,
    OPCODE_ORI = 0b001101,
    OPCODE_XORI = 0b001110,
    OPCODE_LUI = 0b001111,
    OPCODE_FUNCT2 = 0b010001,
    OPCODE_LB = 0b100000,
    OPCODE_LH = 0b100001,
    OPCODE_LWL = 0b100010,
    OPCODE_LW = 0b100011,
    OPCODE_LBU = 0b100100,
    OPCODE_LHU = 0b100101,
    OPCODE_LWR = 0b100110,
    OPCODE_SB = 0b101000,
    OPCODE_SH = 0b101001,
    OPCODE_SWL = 0b101010,
    OPCODE_SW = 0b101011,
    OPCODE_SWR = 0b101110,
    OPCODE_CACHE = 0b101111,
    OPCODE_LL = 0b110000,
    OPCODE_LWC1 = 0b110001,
    OPCODE_LWC2 = 0b110010,
    OPCODE_PREF = 0b110011,
    OPCODE_LDC1 = 0b110101,
    OPCODE_LDC2 = 0b110110,
    OPCODE_SC = 0b111000,
    OPCODE_SWC1 = 0b111001,
    OPCODE_SWC2 = 0b111010,
    OPCODE_SDC1 = 0b111101,
    OPCODE_SDC2 = 0b111110,
};

*/

enum InstructionFormat
{
    R,
    I,
    J,
    FR,
    FI
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

class InstructionFR : Instruction {
private:
    unsigned int m_fmt : 5;
    unsigned int m_ft : 5;
    unsigned int m_fs : 5;
    unsigned int m_fd : 5;
    unsigned int m_funct : 6;

public:

    InstructionFR(unsigned int opcode, unsigned int fmt, unsigned int funct);

    void set_ft(unsigned int ft);
    void set_fs(unsigned int fs);
    void set_fd(unsigned int fd);

    unsigned int get_ft();
    unsigned int get_fs();
    unsigned int get_fd();

    InstructionFormat format() override;

    unsigned int bits() override;
};

class InstructionFI : Instruction {
private:
    unsigned int m_fmt : 5;
    unsigned int m_ft : 5;
    unsigned int m_immediate : 16;

public:

    InstructionFI(unsigned int opcode, unsigned int fmt, unsigned int ft);

    void set_immediate(unsigned int immediate);

    unsigned int get_immediate();

    InstructionFormat format() override;

    unsigned int bits() override;
};