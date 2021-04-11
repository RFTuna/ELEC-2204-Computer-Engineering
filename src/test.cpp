#include "test.hpp"

#include<fstream>
#include"output.hpp"
#include <cstdlib>
#include <chrono>
#include "registers.hpp"
#include "instructions.hpp"

#define DATA0 "$t0"
#define DATA1 "$t1"
#define DATA2 "$t2"
#define DATA3 "$t3"
#define MEMORY "$t4"
#define LOOPTARGET "$t5"
#define LOOPCOUNTER "$t6"

enum LoopType
{
    BLT,
    BGT,
    BLE,
    BGE,

    SLTI,
    SLTIU,
    SLTU,

    LOOPTYPECOUNT,
};

Test::Test(std::string testFilename)
{
    filename = testFilename;
}

unsigned int Test::signExtImm(unsigned int immediate)
{
    return (immediate & 0x00008000 ? 0xffff0000 : 0) + (immediate); 
}

unsigned int Test::zeroExtImm(unsigned int immediate)
{
    return immediate;
}

unsigned int Test::branchAddr(unsigned int immediate)
{
    return (immediate & 0x00008000 ? 0xfffc0000 : 0) + ((immediate) << 2);
}

unsigned int Test::jumpAddr(unsigned int programCounter, unsigned int address)
{
    return (programCounter & 0xF0000000) + (address << 2);
}

void Test::expectR(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, std::string rd, unsigned int shamt)
{

    expected.push_back(std::map<std::string, std::string>());
    expected[expected.size() - 1]["n"] = std::to_string(ic);
    expected[expected.size() - 1]["pc"] = std::to_string(pc);
    expected[expected.size() - 1]["mnemonic"] = mnemonic;
    expected[expected.size() - 1]["opcode"] = std::to_string(Instructions::create(mnemonic)->get_opcode());
    expected[expected.size() - 1]["rs"] = std::to_string(Registers::Number(rs));
    expected[expected.size() - 1]["rt"] = std::to_string(Registers::Number(rt));
    expected[expected.size() - 1]["rd"] = std::to_string(Registers::Number(rd));
    expected[expected.size() - 1]["shamt"] = std::to_string(shamt);
    expected[expected.size() - 1]["funct"] = std::to_string(((InstructionR *)(Instructions::create(mnemonic)))->get_funct());
}

void Test::expectI(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, unsigned int immediate)
{

    expected.push_back(std::map<std::string, std::string>());
    expected[expected.size() - 1]["n"] = std::to_string(ic);
    expected[expected.size() - 1]["pc"] = std::to_string(pc);
    expected[expected.size() - 1]["mnemonic"] = mnemonic;
    expected[expected.size() - 1]["opcode"] = std::to_string(Instructions::create(mnemonic)->get_opcode());
    expected[expected.size() - 1]["rs"] = std::to_string(Registers::Number(rs));
    expected[expected.size() - 1]["rt"] = std::to_string(Registers::Number(rt));
    expected[expected.size() - 1]["immediate"] = std::to_string(immediate);
}

void Test::expectJ(unsigned int ic, unsigned int pc, std::string mnemonic, unsigned int address)
{
    expected.push_back(std::map<std::string, std::string>());
    expected[expected.size() - 1]["n"] = std::to_string(ic);
    expected[expected.size() - 1]["pc"] = std::to_string(pc);
    expected[expected.size() - 1]["mnemonic"] = mnemonic;
    expected[expected.size() - 1]["opcode"] = std::to_string(Instructions::create(mnemonic)->get_opcode());
    expected[expected.size() - 1]["address"] = std::to_string(address);
}

void Test::expectR(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, std::string rd, unsigned int shamt, unsigned int result)
{
    expectR(ic, pc, mnemonic, rs, rt, rd, shamt);
    expected[expected.size() - 1]["result"] = std::to_string(result);
}

void Test::expectI(unsigned int ic, unsigned int pc, std::string mnemonic, std::string rs, std::string rt, unsigned int immediate, unsigned int result)
{
    expectI(ic, pc, mnemonic, rs, rt, immediate);
    expected[expected.size() - 1]["result"] = std::to_string(result);
}

void Test::expectJ(unsigned int ic, unsigned int pc, std::string mnemonic, unsigned int address, unsigned int result)
{
    expectJ(ic, pc, mnemonic, address);
    expected[expected.size() - 1]["result"] = std::to_string(result);
}

void Test::expectNOP(unsigned int ic, unsigned int pc)
{
    expected.push_back(std::map<std::string, std::string>());
    expected[expected.size() - 1]["n"] = std::to_string(ic);
    expected[expected.size() - 1]["pc"] = std::to_string(pc);
    expected[expected.size() - 1]["mnemonic"] = "nop";
}

void Test::expectRegistry(std::string name, unsigned int value)
{
    expected.push_back(std::map<std::string, std::string>());
    expected[expected.size() - 1]["r"] = std::to_string(Registers::Number(name));
    expected[expected.size() - 1]["data"] = std::to_string(value);
}

void Test::expectMemory(unsigned int address, unsigned char value)
{
    expected.push_back(std::map<std::string, std::string>());
    expected[expected.size() - 1]["m"] = std::to_string(address);
    expected[expected.size() - 1]["data"] = std::to_string((unsigned int)value);
}

void Test::expectMemory(unsigned int address, unsigned char value, unsigned int word)
{
    expected.push_back(std::map<std::string, std::string>());
    expected[expected.size() - 1]["m"] = std::to_string(address);
    expected[expected.size() - 1]["data"] = std::to_string((unsigned int)value);
    expected[expected.size() - 1]["word"] = std::to_string(word);
}

void Test::Write()
{
    std::srand(time(NULL));

    std::ofstream file(filename + std::string(".mips"));

    std::map<std::string, unsigned int> registerValues;

    unsigned int pc = 0x00400000;
    unsigned int ic = 0; //count of instructions carried out
    unsigned int ln = 0; //

    //load data
    int data0Start = std::rand() - RAND_MAX / 2;

    file << "   li " << DATA0 << ", " << data0Start << "\n";
    registerValues[DATA0] = data0Start;
    expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[DATA0]) >> 16, 0xFFFF0000 & registerValues[DATA0]);
    pc += 4; ic += 1; ln += 1;
    expectI(ic, pc, "ori", "$at", DATA0, ((int)0xFFFF) & registerValues[DATA0], data0Start);
    pc += 4; ic += 1; ln += 1;

    int data1Start = std::rand() - RAND_MAX / 2;
    file << "   li " << DATA1 << ", " << data1Start << "\n";
    registerValues[DATA1] = data1Start;
    expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[DATA1]) >> 16, 0xFFFF0000 & registerValues[DATA1]);
    pc += 4; ic += 1; ln += 1;
    expectI(ic, pc, "ori", "$at", DATA1, ((int)0xFFFF) & registerValues[DATA1], data1Start);
    pc += 4; ic += 1; ln += 1;

    int data2Start = std::rand() - RAND_MAX / 2;
    file << "   li " << DATA2 << ", " << data2Start << "\n";
    registerValues[DATA2] = data2Start;
    expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[DATA2]) >> 16, 0xFFFF0000 & registerValues[DATA2]);
    pc += 4; ic += 1; ln += 1;
    expectI(ic, pc, "ori", "$at", DATA2, ((int)0xFFFF) & registerValues[DATA2], data2Start);
    pc += 4; ic += 1; ln += 1;

    int data3Start = std::rand() - RAND_MAX / 2;
    file << "   li " << DATA3 << ", " << data3Start << "\n";
    registerValues[DATA3] = data3Start;
    expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[DATA3]) >> 16, 0xFFFF0000 & registerValues[DATA3]);
    pc += 4; ic += 1; ln += 1;
    expectI(ic, pc, "ori", "$at", DATA3, ((int)0xFFFF) & registerValues[DATA3], data3Start);
    pc += 4; ic += 1; ln += 1;

    
    file << "   lui " << MEMORY << ", " << 4096 << "\n";
    registerValues[MEMORY] = 4096 << 16;
    expectI(ic, pc, "lui", MEMORY, MEMORY, 4096, 4096 << 16);
    pc += 4; ic += 1; ln += 1;
    

    LoopType loopType = (LoopType)((unsigned int)std::rand() % LOOPTYPECOUNT);

    if(loopType == SLTU)
    {
        int loopTargetStart = (std::rand() % 100) + 50;
        file << "   li " << LOOPTARGET << ", " << loopTargetStart << "\n";
        registerValues[LOOPTARGET] = loopTargetStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPTARGET]) >> 16, 0xFFFF0000 & registerValues[LOOPTARGET]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPTARGET, ((int)0xFFFF) & registerValues[LOOPTARGET], loopTargetStart);
        pc += 4; ic += 1; ln += 1;

        int loopCounterStart = (std::rand() % 10);
        file << "   li " << LOOPCOUNTER << ", " << loopCounterStart << "\n";
        registerValues[LOOPCOUNTER] = loopCounterStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPCOUNTER]) >> 16, 0xFFFF0000 & registerValues[LOOPCOUNTER]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPCOUNTER, ((int)0xFFFF) & registerValues[LOOPCOUNTER], loopCounterStart);
        pc += 4; ic += 1; ln += 1;
    }
    if(loopType == SLTIU)
    {
        int loopCounterStart = (std::rand() % 10);
        file << "   li " << LOOPCOUNTER << ", " << loopCounterStart << "\n";
        registerValues[LOOPCOUNTER] = loopCounterStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPCOUNTER]) >> 16, 0xFFFF0000 & registerValues[LOOPCOUNTER]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPCOUNTER, ((int)0xFFFF) & registerValues[LOOPCOUNTER], loopCounterStart);
        pc += 4; ic += 1; ln += 1;
    }
    if(loopType == SLTI)
    {
        int loopCounterStart = (std::rand() % 10) - 50;
        file << "   li " << LOOPCOUNTER << ", " << loopCounterStart << "\n";
        registerValues[LOOPCOUNTER] = loopCounterStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPCOUNTER]) >> 16, 0xFFFF0000 & registerValues[LOOPCOUNTER]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPCOUNTER, ((int)0xFFFF) & registerValues[LOOPCOUNTER], loopCounterStart);
        pc += 4; ic += 1; ln += 1;
    }
    else if(loopType == BLT || loopType == BLE)
    {
        int loopTargetStart = (std::rand() % 100) + 50;
        file << "   li " << LOOPTARGET << ", " << loopTargetStart << "\n";
        registerValues[LOOPTARGET] = loopTargetStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPTARGET]) >> 16, 0xFFFF0000 & registerValues[LOOPTARGET]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPTARGET, ((int)0xFFFF) & registerValues[LOOPTARGET], loopTargetStart);
        pc += 4; ic += 1; ln += 1;

        int loopCounterStart = (std::rand() % 10) - 50;
        file << "   li " << LOOPCOUNTER << ", " << loopCounterStart << "\n";
        registerValues[LOOPCOUNTER] = loopCounterStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPCOUNTER]) >> 16, 0xFFFF0000 & registerValues[LOOPCOUNTER]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPCOUNTER, ((int)0xFFFF) & registerValues[LOOPCOUNTER], loopCounterStart);
        pc += 4; ic += 1; ln += 1;
    }
    else if(loopType == BGT || loopType == BGE)
    {
        int loopTargetStart = (std::rand() % 10);
        file << "   li " << LOOPTARGET << ", " << loopTargetStart << "\n";
        registerValues[LOOPTARGET] = loopTargetStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPTARGET]) >> 16, 0xFFFF0000 & registerValues[LOOPTARGET]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPTARGET, ((int)0xFFFF) & registerValues[LOOPTARGET], loopTargetStart);
        pc += 4; ic += 1; ln += 1;

        int loopCounterStart = (std::rand() % 100) + 50;
        file << "   li " << LOOPCOUNTER << ", " << loopCounterStart << "\n";
        registerValues[LOOPCOUNTER] = loopCounterStart;
        expectI(ic, pc, "lui", "$at", "$at", (0xFFFF0000 & registerValues[LOOPCOUNTER]) >> 16, 0xFFFF0000 & registerValues[LOOPCOUNTER]);
        pc += 4; ic += 1; ln += 1;
        expectI(ic, pc, "ori", "$at", LOOPCOUNTER, ((int)0xFFFF) & registerValues[LOOPCOUNTER], loopCounterStart);
        pc += 4; ic += 1; ln += 1;
    }

    std::string arithmeticOperators[] = {"move", "subu", "addiu", "addu", "and", "andi", "nor", "or", "ori", "sll", "srl", "sra"};

    unsigned int num = (((unsigned int)std::rand()) % 100) + 30;

    for(unsigned int i = 0; i < num; i++)
    {
        
        std::string rs = std::rand() % 2 == 0 ? (std::rand() % 2 == 0 ? DATA0 : DATA1) : (std::rand() % 2 == 0 ? DATA2 : DATA3);




        std::string rt = std::rand() % 2 == 0 ? (std::rand() % 2 == 0 ? DATA0 : DATA1) : (std::rand() % 2 == 0 ? DATA2 : DATA3);
        std::string target = std::rand() % 2 == 0 ? (std::rand() % 2 == 0 ? DATA0 : DATA1) : (std::rand() % 2 == 0 ? DATA2 : DATA3);

        std::string op = arithmeticOperators[std::rand() % arithmeticOperators->size()];

        unsigned int result;

        unsigned int immediate = std::rand() % 0xFFFF;
        int negImm = (int)signExtImm(immediate);

        unsigned int shamt = std::rand() % 0x1F;

        if(op == "addu")
            result = registerValues[rs] + registerValues[rt];
        else if(op == "move")
            result = registerValues[rs];
        else if(op == "addiu")
            result = registerValues[rs] + signExtImm(immediate);
        else if(op == "subu")
            result = registerValues[rs] - registerValues[rt];
        else if(op == "and")
            result = registerValues[rs] & registerValues[rt];
        else if(op == "andi")
            result = registerValues[rs] & zeroExtImm(immediate);
        else if(op == "or")
            result = registerValues[rs] | registerValues[rt];
        else if(op == "ori")
            result = registerValues[rs] | zeroExtImm(immediate);
        else if(op == "nor")
            result = ~(registerValues[rs] | registerValues[rt]);
        else if(op == "sll")
            result = registerValues[rt] << shamt;
        else if(op == "srl")
            result = registerValues[rt] >> shamt;
        else if(op == "sra")
            result = ((int)registerValues[rt]) >> shamt;

        if(op == "move")
        {
            file << "   move " << target << ", " << rs << "\n";
            expectR(ic, pc, "addu", rs, "$zero", target, 0, result);
            registerValues[target] = result;
            pc += 4; ic += 1; ln += 1;
        }
        else if(op == "addu" || op == "subu" || op == "and" || op == "or" || op == "nor")
        {
            file << "   " << op << " " << target << ", " << rt << ", " << rs << "\n";
            expectR(ic, pc, op, rs, rt, target, 0, result);
            registerValues[target] = result;
            pc += 4; ic += 1; ln += 1;
        }
        else if(op == "addiu")
        {
            file << "   " << op << " " << target << ", " << rs << ", " << negImm << "\n";
            expectI(ic, pc, op, rs, target, immediate, result);
            registerValues[target] = result;
            pc += 4; ic += 1; ln += 1;
        }
        else if(op == "andi" || op == "ori")
        {
            file << "   " << op << " " << target << ", " << rs << ", " << immediate << "\n";
            expectI(ic, pc, op, rs, target, immediate, result);
            registerValues[target] = result;
            pc += 4; ic += 1; ln += 1;
        }
        else if(op == "sll" || op == "srl" || op == "sra")
        {
            file << "   " << op << " " << target << ", " << rt << ", " << shamt << "\n";
            expectR(ic, pc, op, "$zero", rt, target, shamt, result);
            registerValues[target] = result;
            pc += 4; ic += 1; ln += 1;
        }
    }


    //jump over filler to skip

    //std::string jumpOperators[] = {"j", "jal", "jr"};

    num = (((unsigned int)std::rand()) % 20) + 5;

    file << "   j jump\n";
    expectJ(ic, pc, "j", 0x00100000 + ln + num + 2);
    pc = pc + num * 4 + 8; ic += 1; ln += 1;

    for(unsigned int i = 0; i < num; i++)
    {
        file << "   move $t1, $t2\n";
        ln += 1;
    }
    
    file << "jump:\n";
    ln += 1;



    if(loopType == BLT)
    {
        unsigned int increment = 1;

        
        file << "   " << "addiu" << " " << LOOPCOUNTER << ", " << LOOPCOUNTER << ", " << increment << "\n";
        file << "   blt " <<  LOOPTARGET << ", " << LOOPCOUNTER << ", " << "jump\n";
        ln += 3;

        while(true)
        {
            unsigned int result = registerValues[LOOPCOUNTER] + 1;
            expectI(ic, pc, "addiu", LOOPCOUNTER, LOOPCOUNTER, increment, result);
            registerValues[LOOPCOUNTER] = result;
            pc += 4; ic += 1;

            result = ((int)registerValues[LOOPCOUNTER]) < ((int)registerValues[LOOPTARGET]);
            expectR(ic, pc, "slt", LOOPCOUNTER, LOOPTARGET, "$at", 0, result);
            registerValues["$at"] = result;
            pc += 4; ic += 1;

            expectI(ic, pc, "bne", "$zero", "$at", 0xFFFD);
            ic += 1;
            if(result)
                pc = pc - 8;
            else
            {
                pc += 4;
                break;
            }
        }

        expectNOP(ic, pc);
        pc += 4; ic += 1;
    }
    else if(loopType == BLE)
    {
        unsigned int increment = 1;

        
        file << "   " << "addiu" << " " << LOOPCOUNTER << ", " << LOOPCOUNTER << ", " << increment << "\n";
        file << "   ble " <<  LOOPTARGET << ", " << LOOPCOUNTER << ", " << "jump\n";
        ln += 3;

        while(true)
        {
            unsigned int result = registerValues[LOOPCOUNTER] + 1;
            expectI(ic, pc, "addiu", LOOPCOUNTER, LOOPCOUNTER, increment, result);
            registerValues[LOOPCOUNTER] = result;
            pc += 4; ic += 1;

            result = ((int)registerValues[LOOPTARGET]) < ((int)registerValues[LOOPCOUNTER]);
            expectR(ic, pc, "slt", LOOPTARGET, LOOPCOUNTER, "$at", 0, result);
            registerValues["$at"] = result;
            pc += 4; ic += 1;

            result = !result;
            expectI(ic, pc, "beq", "$zero", "$at", 0xFFFD);
            ic += 1;
            if(result)
                pc = pc - 8;
            else
            {
                pc += 4;
                break;
            }
        }

        expectNOP(ic, pc);
        pc += 4; ic += 1;
    }
    else if(loopType == BGT)
    {    
        file << "   " << "addiu" << " " << LOOPCOUNTER << ", " << LOOPCOUNTER << ", -1" << "\n";
        file << "   bgt " <<  LOOPTARGET << ", " << LOOPCOUNTER << ", " << "jump\n";
        ln += 3;

        while(true)
        {
            unsigned int result = registerValues[LOOPCOUNTER] - 1;
            expectI(ic, pc, "addiu", LOOPCOUNTER, LOOPCOUNTER, 0xFFFF, result);
            registerValues[LOOPCOUNTER] = result;
            pc += 4; ic += 1;

            result = ((int)registerValues[LOOPTARGET]) < ((int)registerValues[LOOPCOUNTER]);
            expectR(ic, pc, "slt", LOOPTARGET, LOOPCOUNTER, "$at", 0, result);
            registerValues["$at"] = result;
            pc += 4; ic += 1;

            expectI(ic, pc, "bne", "$zero", "$at", 0xFFFD);
            ic += 1;
            if(result)
                pc = pc - 8;
            else
            {
                pc += 4;
                break;
            }
        }

        expectNOP(ic, pc);
        pc += 4; ic += 1;
    }
    else if(loopType == BGE)
    {
        file << "   " << "addiu" << " " << LOOPCOUNTER << ", " << LOOPCOUNTER << ", -1" << "\n";
        file << "   bge " <<  LOOPTARGET << ", " << LOOPCOUNTER << ", " << "jump\n";
        ln += 3;

        while(true)
        {
            unsigned int result = registerValues[LOOPCOUNTER] - 1;
            expectI(ic, pc, "addiu", LOOPCOUNTER, LOOPCOUNTER, 0xFFFF, result);
            registerValues[LOOPCOUNTER] = result;
            pc += 4; ic += 1;

            result = ((int)registerValues[LOOPCOUNTER]) < ((int)registerValues[LOOPTARGET]);
            expectR(ic, pc, "slt", LOOPCOUNTER, LOOPTARGET, "$at", 0, result);
            registerValues["$at"] = result;
            pc += 4; ic += 1;

            result = !result;
            expectI(ic, pc, "beq", "$zero", "$at", 0xFFFD);
            ic += 1;
            if(result)
                pc = pc - 8;
            else
            {
                pc += 4;
                break;
            }
        }

        expectNOP(ic, pc);
        pc += 4; ic += 1;
    }
    else if(loopType == SLTU)
    {
        unsigned int increment = 1;

        
        file << "   " << "addiu" << " " << LOOPCOUNTER << ", " << LOOPCOUNTER << ", " << increment << "\n";
        file << "   sltu " <<  "$at" << ", " << LOOPTARGET << ", " << LOOPCOUNTER << "\n";
        file << "   bne " <<  "$at" << ", " << "$zero" << ", " << "jump\n";
        ln += 3;

        while(true)
        {
            unsigned int result = registerValues[LOOPCOUNTER] + 1;
            expectI(ic, pc, "addiu", LOOPCOUNTER, LOOPCOUNTER, increment, result);
            registerValues[LOOPCOUNTER] = result;
            pc += 4; ic += 1;

            result = registerValues[LOOPCOUNTER] < registerValues[LOOPTARGET];
            expectR(ic, pc, "sltu", LOOPCOUNTER, LOOPTARGET, "$at", 0, result);
            registerValues["$at"] = result;
            pc += 4; ic += 1;

            expectI(ic, pc, "bne", "$zero", "$at", 0xFFFD);
            ic += 1;
            if(result)
                pc = pc - 8;
            else
            {
                pc += 4;
                break;
            }
        }

        expectNOP(ic, pc);
        pc += 4; ic += 1;
    }
    else if(loopType == SLTIU)
    {
        unsigned int increment = 1;

        unsigned int target = (std::rand()) % 20 + 70;
        
        file << "   " << "addiu" << " " << LOOPCOUNTER << ", " << LOOPCOUNTER << ", " << increment << "\n";
        file << "   sltiu " <<  "$at" << ", " << LOOPCOUNTER << ", " << target << "\n";
        file << "   bne " <<  "$at" << ", " << "$zero" << ", " << "jump\n";
        ln += 3;

        while(true)
        {
            unsigned int result = registerValues[LOOPCOUNTER] + 1;
            expectI(ic, pc, "addiu", LOOPCOUNTER, LOOPCOUNTER, increment, result);
            registerValues[LOOPCOUNTER] = result;
            pc += 4; ic += 1;

            result = registerValues[LOOPCOUNTER] < target;
            expectI(ic, pc, "sltiu", LOOPCOUNTER, "$at", target, result);
            registerValues["$at"] = result;
            pc += 4; ic += 1;

            expectI(ic, pc, "bne", "$zero", "$at", 0xFFFD);
            ic += 1;
            if(result)
                pc = pc - 8;
            else
            {
                pc += 4;
                break;
            }
        }

        expectNOP(ic, pc);
        pc += 4; ic += 1;
    }
    else if(loopType == SLTI)
    {
        unsigned int increment = 1;

        unsigned int target = (std::rand()) % 20 + 70;
        
        file << "   " << "addiu" << " " << LOOPCOUNTER << ", " << LOOPCOUNTER << ", " << increment << "\n";
        file << "   slti " <<  "$at" << ", " << LOOPCOUNTER << ", " << target << "\n";
        file << "   bne " <<  "$at" << ", " << "$zero" << ", " << "jump\n";
        ln += 3;

        while(true)
        {
            unsigned int result = registerValues[LOOPCOUNTER] + 1;
            expectI(ic, pc, "addiu", LOOPCOUNTER, LOOPCOUNTER, increment, result);
            registerValues[LOOPCOUNTER] = result;
            pc += 4; ic += 1;

            result = ((int)registerValues[LOOPCOUNTER]) < ((int)target);
            expectI(ic, pc, "slti", LOOPCOUNTER, "$at", target, result);
            registerValues["$at"] = result;
            pc += 4; ic += 1;

            expectI(ic, pc, "bne", "$zero", "$at", 0xFFFD);
            ic += 1;
            if(result)
                pc = pc - 8;
            else
            {
                pc += 4;
                break;
            }
        }

        expectNOP(ic, pc);
        pc += 4; ic += 1;
    }



    file << "   jal " << "memtest\n";
    registerValues["$ra"] = pc + 8;
    expectJ(ic, pc, "jal", 0x00100000 + ln + 4, registerValues["$ra"]);
    pc = pc + 12; 
    ic += 1; ln += 1;

    file << "   syscall\n";
    file << "memtest:\n";

    std::map<unsigned int, unsigned char> memoryValues;


    file << "   sb " << DATA0 << " 0(" << MEMORY << ")\n";
    memoryValues[registerValues[MEMORY] + 0] = 0xFF & registerValues[DATA0];
    expectI(ic, pc, "sb", MEMORY, DATA0, 0);
    pc += 4; ic += 1; ln += 1;

    file << "   sh " << DATA1 << " 4(" << MEMORY << ")\n";
    memoryValues[registerValues[MEMORY] + 4] = 0xFF & registerValues[DATA1];
    memoryValues[registerValues[MEMORY] + 5] = (0xFF00 & registerValues[DATA1]) >> 8;
    expectI(ic, pc, "sh", MEMORY, DATA1, 4);
    pc += 4; ic += 1; ln += 1;

    file << "   sw " << DATA2 << " 8(" << MEMORY << ")\n";
    memoryValues[registerValues[MEMORY] + 8] = 0xFF & registerValues[DATA2];
    memoryValues[registerValues[MEMORY] + 9] = (0xFF00 & registerValues[DATA2]) >> 8;
    memoryValues[registerValues[MEMORY] + 10] = (0xFF0000 & registerValues[DATA2]) >> 16;
    memoryValues[registerValues[MEMORY] + 11] = (0xFF000000 & registerValues[DATA2]) >> 24;
    expectI(ic, pc, "sw", MEMORY, DATA2, 8);
    pc += 4; ic += 1; ln += 1;

    file << "   lbu " << DATA1 << " 0(" << MEMORY << ")\n";
    unsigned int result = memoryValues[registerValues[MEMORY] + 0];
    registerValues[DATA1] = result;
    expectI(ic, pc, "lbu", MEMORY, DATA1, 0, result);
    pc += 4; ic += 1; ln += 1;

    file << "   lhu " << DATA2 << " 4(" << MEMORY << ")\n";
    result = 
        (memoryValues[registerValues[MEMORY] + 5] << 8) + 
        memoryValues[registerValues[MEMORY] + 4];
    registerValues[DATA2] = result;
    expectI(ic, pc, "lhu", MEMORY, DATA2, 4, result);
    pc += 4; ic += 1; ln += 1;

    file << "   lw " << DATA0 << " 8(" << MEMORY << ")\n";
    result = 
        (memoryValues[registerValues[MEMORY] + 11] << 24) + 
        (memoryValues[registerValues[MEMORY] + 10] << 16) + 
        (memoryValues[registerValues[MEMORY] + 9] << 8) + 
        memoryValues[registerValues[MEMORY] + 8];
    registerValues[DATA0] = result;
    expectI(ic, pc, "lw", MEMORY, DATA0, 8, result);
    pc += 4; ic += 1; ln += 1;



    file << "   jr $ra";
    expectR(ic, pc, "jr", "$ra","$ra", "$ra", 0);
    pc = registerValues["$ra"]; ic += 1; ln += 1;

    expectR(ic, pc, "syscall", "$zero","$zero", "$zero", 0);

    for(unsigned int i = 0; i < 32; i++)
    { 
        bool found = false;

        for ( const auto &pair : registerValues ) {
            if(pair.first == Registers::Name(i))
            {
                expectRegistry(pair.first, pair.second);
                found = true;
            }
        }

        if(!found)
            expectRegistry(Registers::Name(i), 0);
    }

    for ( const auto &pair : memoryValues ) {
        if(pair.first >= (unsigned int)0x10000000)
        {
            if(pair.first % 4 == 0)
            {
                unsigned int word = memoryValues[pair.first] + 
                    (memoryValues[pair.first + 1] << 8) +
                    (memoryValues[pair.first + 2] << 16) +
                    (memoryValues[pair.first + 3] << 24);
                expectMemory(pair.first, pair.second, word);
            }
            else
            {
                expectMemory(pair.first, pair.second);
            }
        }
    }
}

bool Test::Check()
{
    std::ifstream file(filename + std::string(".log"));

    if(file.is_open())
    {
        std::string line;
        unsigned int lineNumber = 0;
        while(std::getline(file, line))
        {
            unsigned int linePos;
            std::string pair;

            while((linePos = line.find(" | ")) != std::string::npos)
            {
                pair = line.substr(0, linePos);
                line.erase(0, linePos + 3);

                unsigned int pos = pair.find(": ");
                std::string key = pair.substr(0, pos);
                std::string value = pair.substr(pos + 2);

                if(lineNumber >= expected.size())
                {
                    Output::DebugLine("");
                    Output::Debug("Test failed, log longer than expected");

                    Output::PrintLine("Errors found :(");

                    return false;
                }

                if(expected[lineNumber][key] != value)
                {
                    Output::DebugLine("");
                    Output::Debug("Test failed on line ");
                    Output::Debug(lineNumber);
                    Output::Debug(" key: ");
                    Output::DebugLine(key);

                    if(expected[lineNumber][key] == "")
                    {
                        Output::DebugLine("Did not expect key");     
                    }
                    else {
                        Output::Debug("Expected ");
                        Output::Debug(expected[lineNumber][key]);
                        Output::Debug(" found ");
                        Output::DebugLine(value);
                    }

                    Output::PrintLine("Errors found :(");

                    return false;
                }
                
            }

            if(lineNumber < expected.size())
            {
                pair = line;

                unsigned int pos = pair.find(": ");
                std::string key = pair.substr(0, pos);
                std::string value = pair.substr(pos + 2);

                if(lineNumber >= expected.size())
                {
                    Output::DebugLine("");
                    Output::Debug("Test failed, log longer than expected");

                    Output::PrintLine("Errors found :(");

                    return false;
                }

                if(expected[lineNumber][key] != value)
                {
                    Output::DebugLine("");
                    Output::Debug("Test failed on line ");
                    Output::Debug(lineNumber);
                    Output::Debug(" key: ");
                    Output::DebugLine(key);

                    if(expected[lineNumber][key] == "")
                    {
                        Output::DebugLine("Did not expect key");     
                    }
                    else {
                        Output::Debug("Expected ");
                        Output::Debug(expected[lineNumber][key]);
                        Output::Debug(" found ");
                        Output::DebugLine(value);
                    }

                    Output::PrintLine("Errors found :(");

                    return false;
                }
            }
            lineNumber++;
        }

        Output::DebugLine("");
        Output::DebugLine("Test successful");

        Output::PrintLine("No errors found! :)");

        return true;
    }

    return false;
}