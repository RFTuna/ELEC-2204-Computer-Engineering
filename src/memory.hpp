#pragma once

#include <map>

class Memory {

//simulate 4gb memory by adding data dynamically as it is used
private:

std::map<unsigned int, unsigned char> data;

public:

unsigned char getByte(unsigned int address);
void setByte(unsigned int address, unsigned char value);

unsigned int getHalfword(unsigned int address);
void setHalfword(unsigned int address, unsigned int value);

unsigned int getWord(unsigned int address);
void setWord(unsigned int address, unsigned int value);

void Debug();

};