#pragma once

#include <vector>
#include <map>
#include <string>

class Test
{
private:

std::vector<std::map<std::string, std::string>> expected;

void expectR(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, std::string rd, unsigned int shamt);
void expectR(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, std::string rd, unsigned int shamt, unsigned int result);

void expectI(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, unsigned int immediate);
void expectI(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, unsigned int immediate, unsigned int result);

void expectJ(unsigned int ic, unsigned int pc, std::string mnemonic, unsigned int address);
void expectJ(unsigned int ic, unsigned int pc, std::string mnemonic, unsigned int address, unsigned int result);

void expectNOP(unsigned int ic, unsigned int pc);

void expectRegistry(std::string name, unsigned int value);

void expectMemory(unsigned int address, unsigned char value);

unsigned int signExtImm(unsigned int immediate);
unsigned int zeroExtImm(unsigned int immediate);
unsigned int branchAddr(unsigned int immediate);
unsigned int jumpAddr(unsigned int programCounter, unsigned int address);

std::string filename;

public:

Test(std::string testFilename);

void Write();

bool Check();

};