#include "Scanner.h"
#include <iostream>
#include "DebuggerLog.h"

char Scanner::peek()
{
    if(pos >= input.size()-1) return '\0';
    return input[pos+1];
}

char Scanner::advance()
{
    char temp=input[pos++];
    return temp;
}

void Scanner::skipSpace()
{
    while(pos<input.size()&&input[pos]==' ')pos++;
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
    while (pos < input.size() && (isalpha(input[pos])||input[pos]=='.'))
    {
        temp+=input[pos++];
    }
    DB("We got a word: "+temp +"|");
    return temp;
}

bool Scanner::isGreLessThan(const char&c)
{
    if(c=='>'||c=='<')return true;
    return false;
}

bool Scanner::isAtEnd()
{
    if(pos>=input.size())return true;
    return false;
}

// std::vector<Token> Scanner::scanToken()
// {
//     if(input.empty())
//     {
//         std::cout<<"No Input"<<std::endl;
//     }

//     std::vector<Token> Tokens;
    
//     size_t len=input.size();

//     bool isSingleQ=false;
//     bool isDoubleQ=false;
//     bool isEscapeSeq=false;
//     skipSpace();
//     while (pos<len)
//     {
//         char c=input[pos];
//         if(isalpha(c))
//         {
//             Tokens.push_back(creatToken(TokenType::WORD,getWord()));
//         }
//         else if(isdigit(c) && (pos<len-1)&& isGreLessThan(peek()))
//         {
//             std::string temp="";temp+=advance();temp+=advance();
//             // std::cout<<"We get into"<<std::endl;
//             Tokens.push_back(creatToken(TokenType::IO_NUMBER,temp));
//         }
//         else if(isGreLessThan(c))
//         {
//             Token temp;
//             if(c=='<') temp = creatToken(TokenType::REDIRECT_IN, "<");
//             else if(c=='>') temp = creatToken(TokenType::REDIRECT_OUT, ">");
//             Tokens.push_back(temp);
//             pos++;
//         }
//         else if(c=='\''||c=='\"'||c=='\\'||c=='/')
//         {
//             DB("We are in the string char is "+std::string(1,c));
//             isSingleQ = false;
//             isDoubleQ = false;
//             isEscapeSeq = false;
//             std::string currtok="";
//             while(pos<len)
//             {
//                 char tem=input[pos];
//                 if(tem=='\''&&!isDoubleQ&&!isEscapeSeq)isSingleQ=!isSingleQ;
//                 else if(tem=='\"'&&!isSingleQ&&!isEscapeSeq)isDoubleQ=!isDoubleQ;
//                 else if(tem==' '&&!isSingleQ&&!isDoubleQ&&!isEscapeSeq)
//                 {
//                     if(!currtok.empty())Tokens.push_back(creatToken(TokenType::WORD,currtok));
//                     pos++;
//                     currtok.clear();
//                     break;
//                 }
//                 else if(tem=='\\'&&!isEscapeSeq&&!isSingleQ)isEscapeSeq=!isEscapeSeq;
//                 else
//                 {
//                     if(isEscapeSeq)
//                     {
//                         if(isDoubleQ)
//                         {
//                             if(!(tem=='\\'||tem=='$'||tem=='\"'))currtok+='\\';
//                         }
//                         isEscapeSeq=false;
//                     }
                    
//                     // if((currtok.empty())&&tem=='>')currtok+='1';
//                     DB("Adding character to current token: "+std::string(1,tem));
//                     currtok+=tem;
//                 }
//                 pos++;
//             }
//             if(!currtok.empty())Tokens.push_back(creatToken(TokenType::WORD,currtok));
//         }
//         else
//         {
//             std::string temp="";
//             while (!isAtEnd()&&input[pos]!=' ') temp+=advance();
//             Tokens.push_back(creatToken(TokenType::WORD,temp));
//         }
//         skipSpace();
//     }
//     return Tokens;
// }



std::vector<Token> Scanner::scanToken()
{
    if(input.empty())
    {
        std::cout<<"No Input"<<std::endl;
        return {}; // Return empty vector
    }

    std::vector<Token> Tokens;
    size_t len=input.size();

    while (pos < len)
    {
        skipSpace();
        if (isAtEnd()) break;

        char c = input[pos];

        if (isdigit(c) && (pos < len-1) && isGreLessThan(peek()))
        {
            std::string temp = "";
            temp += advance();
            Tokens.push_back(creatToken(TokenType::IO_NUMBER, temp));
            continue;
        }
        else if (isGreLessThan(c))
        {
            Token temp;
            if (c == '<')
            {
                if(peek()=='<')
                {
                    temp= creatToken(TokenType::REDIRECT_IN_APP, "<<");
                    pos++; // Skip the next '<'
                }
                else temp = creatToken(TokenType::REDIRECT_IN, "<");
            }
            else if (c == '>')
            {
                if(peek()=='>')
                {
                    temp= creatToken(TokenType::REDIRECT_OUT_APP, ">>");
                    pos++; // Skip the next '>'
                }
                else temp = creatToken(TokenType::REDIRECT_OUT, ">");
            }
            Tokens.push_back(temp);
            pos++;
            continue;
        }
        
        bool isSingleQ = false;
        bool isDoubleQ = false;
        bool isEscapeSeq = false;
        std::string currtok = "";

        while (pos < len)
        {
            char tem = input[pos];

            if ((tem == ' ' || isGreLessThan(tem)) && !isSingleQ && !isDoubleQ && !isEscapeSeq)
            {
                break; 
            }

            if (isEscapeSeq) {
                if (isDoubleQ) {
                    if (tem != '\"' && tem != '$' && tem != '\\') {
                        currtok += '\\';
                    }
                }
                currtok += tem;
                isEscapeSeq = false;
            }
            else {
                if (tem == '\'' && !isDoubleQ) isSingleQ = !isSingleQ;
                else if (tem == '\"' && !isSingleQ) isDoubleQ = !isDoubleQ;
                else if (tem == '\\' && !isSingleQ) isEscapeSeq = true;
                else currtok += tem;
            }
            pos++;
        }

        if (!currtok.empty())
        {
            Tokens.push_back(creatToken(TokenType::WORD, currtok));
        }
    }
    return Tokens;
}