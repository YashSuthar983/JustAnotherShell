#pragma once

#include <string>
#include <vector>
#include "Token.h"

class Scanner
{
    std::string input;
    size_t pos;

    char peek();
    char advance();
    void skipSpace();
    std::string getWord();
    bool isGreLessThan();
    
    Token creatToken(TokenType type,const std::string & std="");

    public:
    bool isAtEnd();
    Scanner(std::string& input) : input(input){pos=0;}
    std::vector<Token> scanToken();
};