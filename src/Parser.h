#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Token.h"
#include "InBuilt.h"



class Parser {
    std::vector<Token> Tokens;
    
    std::vector<std::string> currentArgs;

    Token peek();
    Token previous();
    Token advance();
    bool isAtEnd();
    bool check(TokenType type);

    public:
    Parser(std::vector<Token> Tokens) : Tokens(std::move(Tokens)) {}
    std::shared_ptr<Command> parseSimpleComm();
    std::shared_ptr<Command> parseCommand();
    std::shared_ptr<Command> parse();
};