#include "parser.hpp"

#include "output.hpp"

#include <fstream>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <map>

#include "registers.hpp"

std::string Parser::pseudoInstructions[]
{
    "blt",
    "bgt",
    "ble",
    "bge",
    "li",
    "move",
    //not pseudo commands
    "syscall",
    "lui",
};

std::vector<Lexeme> Parser::lex(std::string filename)
{
    std::ifstream file(filename);

    std::vector<Lexeme> lexemes;

    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            //remove comment
            size_t commentStart = line.find('#');

            if(commentStart != std::string::npos)
                line = line.substr(0, commentStart);

            //remove excess characters
            line.erase(std::remove(line.begin(), line.end(), ','), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
            line.erase(std::remove(line.begin(), line.end(), ')'), line.end());
            std::replace(line.begin(), line.end(), '(', ' ');


            //split
            std::vector<std::string> tokens;

            std::istringstream iss(line);
            std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(tokens));

            //proccess tokens
            if(tokens.size() > 0)
            {
                if(tokens[0][tokens[0].length() - 1] == ':')
                {
                    lexemes.push_back({tokens[0].substr(0, tokens[0].length() - 1), LABEL});
                }
                else
                {
                    lexemes.push_back({tokens[0], COMMAND});

                    for(unsigned int i = 1; i < tokens.size(); i++)
                        lexemes.push_back({tokens[i], ARGUMENT});
                }
            }
        }
    }

    return lexemes;
}

void Parser::handlePseudo(std::vector<Lexeme> &lexemes)
{
    for(int i = lexemes.size() - 1; i >= 0; i--)
    {
        if(lexemes[i].type == COMMAND && std::find(std::begin(pseudoInstructions), std::end(pseudoInstructions), lexemes[i].token) != std::end(pseudoInstructions))
        {
            std::vector<Lexeme> args;

            int arg = i + 1;
            while(lexemes[arg].type == ARGUMENT)
            {
                args.push_back(lexemes[arg]);
                lexemes.erase(lexemes.begin() + arg);
            }

            if(lexemes[i].token == "blt")
            {
                lexemes[i] = {"slt", COMMAND};
                lexemes.insert(lexemes.begin() + i + 1, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 2, args[0]);
                lexemes.insert(lexemes.begin() + i + 3, args[1]);

                lexemes.insert(lexemes.begin() + i + 4, {"bne", COMMAND});
                lexemes.insert(lexemes.begin() + i + 5, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 6, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 7, args[2]);
            }
            else if(lexemes[i].token == "ble")
            {
                lexemes[i] = {"slt", COMMAND};
                lexemes.insert(lexemes.begin() + i + 1, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 2, args[1]);
                lexemes.insert(lexemes.begin() + i + 3, args[0]);

                lexemes.insert(lexemes.begin() + i + 4, {"beq", COMMAND});
                lexemes.insert(lexemes.begin() + i + 5, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 6, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 7, args[2]);
            }
            else if(lexemes[i].token == "bgt")
            {
                lexemes[i] = {"slt", COMMAND};
                lexemes.insert(lexemes.begin() + i + 1, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 2, args[1]);
                lexemes.insert(lexemes.begin() + i + 3, args[0]);

                lexemes.insert(lexemes.begin() + i + 4, {"bne", COMMAND});
                lexemes.insert(lexemes.begin() + i + 5, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 6, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 7, args[2]);
            }
            else if(lexemes[i].token == "bge")
            {
                lexemes[i] = {"slt", COMMAND};
                lexemes.insert(lexemes.begin() + i + 1, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 2, args[0]);
                lexemes.insert(lexemes.begin() + i + 3, args[1]);

                lexemes.insert(lexemes.begin() + i + 4, {"beq", COMMAND});
                lexemes.insert(lexemes.begin() + i + 5, {"$at", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 6, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 7, args[2]);
            }
            else if(lexemes[i].token == "li")
            {
                lexemes[i] = {"ori", COMMAND};
                lexemes.insert(lexemes.begin() + i + 1, args[0]);
                lexemes.insert(lexemes.begin() + i + 2, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 3, args[1]);
            }
            else if(lexemes[i].token == "move")
            {
                lexemes[i] = {"addu", COMMAND};
                lexemes.insert(lexemes.begin() + i + 1, args[0]);
                lexemes.insert(lexemes.begin() + i + 2, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 3, args[1]);
            }
            else if(lexemes[i].token == "syscall") //not pseudo but needs filler
            {
                lexemes.insert(lexemes.begin() + i + 1, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 2, {"$zero", ARGUMENT});
                lexemes.insert(lexemes.begin() + i + 3, {"$zero", ARGUMENT});
            }
            else if(lexemes[i].token == "lui") //not pseudo but needs filler
            {
                lexemes.insert(lexemes.begin() + i + 1, args[0]);
                lexemes.insert(lexemes.begin() + i + 2, args[0]);
                lexemes.insert(lexemes.begin() + i + 3, args[1]);
            }
        }
    }
}

std::vector<Instruction *>Parser::parse(std::string filename)
{
    std::vector<Instruction *> instructions;

    std::vector<Lexeme> lexemes = lex(filename);

    handlePseudo(lexemes);

    //memory operands $t0 offset(base register)
    //use shamt for shift functions only

    std::map<std::string, unsigned int> labelPositions;

    for(unsigned int i = 0; i < lexemes.size(); i++)
    {
        if(lexemes[i].type == COMMAND && (lexemes[i].token == "beq" || lexemes[i].token == "bne"))
            lexemes.insert(lexemes.begin() + i + 4, {"nop", COMMAND});
        if(lexemes[i].type == COMMAND && (lexemes[i].token == "j" || lexemes[i].token == "jal" || lexemes[i].token == "jr"))
            lexemes.insert(lexemes.begin() + i + 3, {"nop", COMMAND});
    }

    unsigned int commandCount = 0;

    for(unsigned int i = 0; i < lexemes.size(); i++)
    {
        if(lexemes[i].type == COMMAND)
            commandCount++;

        if(lexemes[i].type == LABEL)
        {
            labelPositions[lexemes[i].token] = commandCount;
        }
    }

    commandCount = 0;

    for(unsigned int i = 0; i < lexemes.size(); i++)
    {
        if(lexemes[i].type == COMMAND)
        {
            Instruction *instruction = Instructions::create(lexemes[i].token);

            if(instruction->bits() == 0);
            else if(instruction->format() == R)
            {
                ((InstructionR *)instruction)->set_rd(Registers::Number(lexemes[i + 1].token));
                ((InstructionR *)instruction)->set_rt(Registers::Number(lexemes[i + 2].token));
                if(lexemes[i].token == "sll" || lexemes[i].token == "srl" || lexemes[i].token == "sra")
                    ((InstructionR *)instruction)->set_shamt(std::stoi(lexemes[i + 3].token));
                else
                    ((InstructionR *)instruction)->set_rs(Registers::Number(lexemes[i + 3].token));
            }
            else if(instruction->format() == I)
            {
                ((InstructionI *)instruction)->set_rt(Registers::Number(lexemes[i + 1].token));
                if(lexemes[i].token == "lbu" || lexemes[i].token == "lhu" || lexemes[i].token == "lw" || lexemes[i].token == "sb" || lexemes[i].token == "sh" || lexemes[i].token == "sw")
                {
                    ((InstructionI *)instruction)->set_immediate(std::stoi(lexemes[i + 2].token));
                    ((InstructionI *)instruction)->set_rs(Registers::Number(lexemes[i + 3].token));
                }
                else
                {
                    ((InstructionI *)instruction)->set_rs(Registers::Number(lexemes[i + 2].token));
                    if(lexemes[i].token == "beq" || lexemes[i].token == "bne")
                    {
                        ((InstructionI *)instruction)->set_immediate(labelPositions[lexemes[i + 3].token] - commandCount - 1);
                    }
                    else
                        ((InstructionI *)instruction)->set_immediate(std::stoi(lexemes[i + 3].token));
                }
            }
            else if(instruction->format() == J)
            {
                ((InstructionJ *)instruction)->set_address(labelPositions[lexemes[i + 1].token] + 0x00400000);
            }

            instructions.push_back(instruction);

            commandCount++;
        }
    }

    return instructions;
}