#include "instruction.hpp"

#include "instructions.hpp"

#include <string>

unsigned int Instruction::get_opcode()
{
    return m_opcode;
}

std::string Instruction::mnemonic()
{
    return Instructions::mnemonic(bits());
}

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

unsigned int InstructionR::get_funct()
{
    return m_funct;
}

InstructionFormat InstructionR::format()
{
    return R;
}

unsigned int InstructionR::bits()
{
    return (m_opcode << 26) + (m_rs << 21) + (m_rt << 16) + (m_rd << 11)  + (m_shamt << 6) + m_funct;
}

std::string InstructionR::outputString()
{
    if(mnemonic() == "nop")
        return "nop";

    return std::string("mnemonic: ")
    .append(mnemonic())
    .append(" | opcode: ").append(std::to_string(m_opcode))
    .append(" | rs: ").append(std::to_string(m_rs))
    .append(" | rt: ").append(std::to_string(m_rt))
    .append(" | rd: ").append(std::to_string(m_rd))
    .append(" | shamt: ").append(std::to_string(m_shamt))
    .append(" | funct: ").append(std::to_string(m_funct));
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

std::string InstructionI::outputString()
{
    return std::string("mnemonic: ")
    .append(mnemonic())
    .append(" | opcode: ").append(std::to_string(m_opcode))
    .append(" | rs: ").append(std::to_string(m_rs))
    .append(" | rt: ").append(std::to_string(m_rt))
    .append(" | immediate: ").append(std::to_string(m_immediate));
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

std::string InstructionJ::outputString()
{
    return std::string("mnemonic: ")
    .append(mnemonic())
    .append(" | opcode: ").append(std::to_string(m_opcode))
    .append(" | address: ").append(std::to_string(m_address));
}