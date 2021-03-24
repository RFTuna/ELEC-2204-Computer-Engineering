#pragma once

#include <string>
#include "instructions.hpp"
#include <vector>

enum TokenType
{
    LABEL,
    COMMAND,
    ARGUMENT,
};

struct Lexeme
{
    std::string token;
    TokenType type;
};

class Parser
{
private:
static std::string pseudoInstructions[];

static std::vector<Lexeme> lex(std::string filename);
static void handlePseudo(std::vector<Lexeme> &lexemes);
public:
static std::vector<Instruction *>parse(std::string filename);
};