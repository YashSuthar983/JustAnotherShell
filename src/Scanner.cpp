#include "Scanner.h"
#include <iostream>

char Scanner::peek()
{
    if(pos>=input.size()-1);
    return input[pos+1];
}

char Scanner::advance()
{
    char temp=input[pos++];
    return temp;
}

void Scanner::skipSpace()
{
    while(input[pos]==' '&&pos<input.size())pos++;
}

Token Scanner::creatToken(TokenType type, const std::string & value)
{
    Token token;
    token.type = type;
    token.value = value;
    return token;
}

std::string Scanner::getWord()
{
    std::string temp="";
    while (isalpha(input[pos])||input[pos]=='.')
    {
        temp+=input[pos++];
    }
    return temp;
}

bool Scanner::isGreLessThan()
{
    char c=peek();
    if(c=='>'||c=='<')return true;
    return false;
}

bool Scanner::isAtEnd()
{
    if(pos>=input.size())return true;
    return false;
}

std::vector<Token> Scanner::scanToken()
{
    if(input.empty())
    {
        std::cout<<"No Input"<<std::endl;
    }

    std::vector<Token> Tokens;
    
    size_t len=input.size();

    bool isSingleQ=false;
    bool isDoubleQ=false;
    bool isEscapeSeq=false;
    while (pos<len)
    {
        char c=input[pos];
        
        if(isalpha(c))
        {
            Tokens.push_back(creatToken(TokenType::WORD,getWord()));
        }
        else if(isdigit(c)&&isGreLessThan())
        {
            std::string temp="";temp+=advance();temp+=advance();
            std::cout<<"We get into"<<std::endl;
            Tokens.push_back(creatToken(TokenType::IO_NUMBER,temp));
        }
        else if(isGreLessThan())
        {
            Token temp;
            if(c=='<') temp = creatToken(TokenType::LESS_THAN, "<");
            else if(c=='>') temp = creatToken(TokenType::GREATER_THAN, ">");
            Tokens.push_back(temp);
            pos++;
        }
        else if(c=='\''||c=='\"'||c=='\\'||c=='/')
        {
            std::string currtok="";
            while(pos<len)
            {
                char tem=input[pos];
                if(tem=='\''&&!isDoubleQ&&!isEscapeSeq)isSingleQ=!isSingleQ;
                else if(tem=='\"'&&!isSingleQ&&!isEscapeSeq)isDoubleQ=!isDoubleQ;
                else if(tem==' '&&!isSingleQ&&!isDoubleQ&&!isEscapeSeq)
                {
                    if(!currtok.empty())Tokens.push_back(creatToken(TokenType::WORD,currtok));
                    currtok.clear();
                }
                else if(tem=='\\'&&!isEscapeSeq&&!isSingleQ)isEscapeSeq=!isEscapeSeq;
                else
                {
                    if(isEscapeSeq)
                    {
                        if(isDoubleQ)
                        {
                            if(!(tem=='\\'||tem=='$'||tem=='\"'))currtok+='\\';
                        }
                        isEscapeSeq=false;
                    }
                    
                    if((!currtok.empty())&&tem=='>')currtok+='1';
                    currtok+=tem;
                }
                pos++;
            }
            if(!currtok.empty())Tokens.push_back(creatToken(TokenType::WORD,currtok));
        }
        else
        {
            std::string temp="";
            while (!isAtEnd()&&input[pos]!=' ') temp+=advance();
            Tokens.push_back(creatToken(TokenType::WORD,temp));
        }
        skipSpace();
    }
    return Tokens;
}