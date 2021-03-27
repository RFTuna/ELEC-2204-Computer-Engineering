#include "memory.hpp"

unsigned char Memory::get(unsigned int address)
{
    if(address >= 0x04000000 && address < 0x80000000)
    {
        if(!data.count(address))
            data[address] = 0;
        return address;
    }
    else return 0;
}
void Memory::set(unsigned int address, unsigned char value)
{
    if(address >= 0x04000000 && address < 0x80000000)
    {
        data[address] = value;
    }
}