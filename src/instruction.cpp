#include "instruction.hpp"

InstructionR::InstructionR(unsigned int opcode, unsigned int funct)
{
    m_opcode = opcode;
    m_funct = funct;
    m_rs = 0;
    m_rt = 0;
    m_rd = 0;
    m_shamt = 0;
}

void InstructionR::set_rs(unsigned int rs)
{
    m_rs = rs;
}

void InstructionR::set_rt(unsigned int rt)
{
    m_rt = rt;
}

void InstructionR::set_rd(unsigned int rd)
{
    m_rd = rd;
}

void InstructionR::set_shamt(unsigned int shamt)
{
    m_shamt = shamt;
}

unsigned int InstructionR::get_rs()
{
    return m_rs;
}

unsigned int InstructionR::get_rt()
{
    return m_rt;
}

unsigned int InstructionR::get_rd()
{
    return m_rd;
}

unsigned int InstructionR::get_shamt()
{
    return m_shamt;
}

InstructionFormat InstructionR::format()
{
    return R;
}

unsigned int InstructionR::bits()
{
    return (m_opcode << 26) + (m_rs << 21) + (m_rt << 16) + (m_rd << 11)  + (m_shamt << 6) + m_funct;
}

InstructionI::InstructionI(unsigned int opcode)
{
    m_opcode = opcode;
    m_rs = 0;
    m_rt = 0;
    m_immediate = 0;
}

void InstructionI::set_rs(unsigned int rs)
{
    m_rs = rs;
}

void InstructionI::set_rt(unsigned int rt)
{
    m_rt = rt;
}

void InstructionI::set_immediate(unsigned int immediate)
{
    m_immediate = immediate;
}

unsigned int InstructionI::get_rs()
{
    return m_rs;
}

unsigned int InstructionI::get_rt()
{
    return m_rt;
}

unsigned int InstructionI::get_immediate()
{
    return m_immediate;
}


InstructionFormat InstructionI::format()
{
    return I;
}

unsigned int InstructionI::bits()
{
    return (m_opcode << 26) + (m_rs << 21) + (m_rt << 16) + m_immediate;
}

InstructionJ::InstructionJ(unsigned int opcode)
{
    m_opcode = opcode;
    m_address = 0;
}

void InstructionJ::set_address(unsigned int address)
{
    m_address = address;
}

unsigned int InstructionJ::get_address()
{
    return m_address;
}

InstructionFormat InstructionJ::format()
{
    return J;
}

unsigned int InstructionJ::bits()
{
    return (m_opcode << 26) + m_address;
}