#include "memory.hpp"

#include "output.hpp"

unsigned char Memory::getByte(unsigned int address)
{
    if(address >= 0x04000000 && address < 0x80000000)
    {
        if(!data.count(address))
            data[address] = 0;
        return data[address];
    }
    else return 0;
}

void Memory::setByte(unsigned int address, unsigned char value)
{
    if(address >= 0x04000000 && address < 0x80000000)
    {
        data[address] = value;
    }
}

unsigned int Memory::getHalfword(unsigned int address)
{
    return ((unsigned int)getByte(address + 1) << 8) + getByte(address);
}

void Memory::setHalfword(unsigned int address, unsigned int value)
{
    setByte(address, value & 0x000000FF);
    setByte(address + 1, (value & 0x0000FF00) >> 8);
}

unsigned int Memory::getWord(unsigned int address)
{
    return ((unsigned int)getHalfword(address + 2) << 16) + getHalfword(address);
}

void Memory::setWord(unsigned int address, unsigned int value)
{
    setHalfword(address, value & 0x0000FFFF);
    setHalfword(address + 2, (value & 0xFFFF0000) >> 16);
}