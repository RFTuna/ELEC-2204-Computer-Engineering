#pragma once

#include <map>

class Memory {

//simulate 4gb memory by adding data dynamically as it is used
private:

std::map<unsigned int, unsigned char> data;

public:

unsigned char get(unsigned int address);
void set(unsigned int address, unsigned char value);

};