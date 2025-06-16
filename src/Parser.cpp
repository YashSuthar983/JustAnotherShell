#include "Parser.h"
#include "Global.h"
#include <sys/wait.h>
#include <unistd.h>

Token Parser::peek()
{
    if(curr < Tokens.size()) return Tokens[curr];
    return Token{TokenType::TOKEN_EOF, ""};
}

Token Parser::previous()
{
    if(curr > 0) return Tokens[curr-1];
    return Token{TokenType::TOKEN_EOF, ""};
}

Token Parser::advance()
{
    if(!isAtEnd()) return Tokens[curr++];
    return Token{TokenType::TOKEN_EOF, ""};
}

bool Parser::check(TokenType type)
{
    if(isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::isAtEnd()
{
    return curr >= Tokens.size();
}

std::shared_ptr<Command> Parser::parseSimpleComm() {
    if(!check(TokenType::WORD)) return nullptr;

    currentArgs.clear();
    Token cmdToken = advance();
    std::string name = cmdToken.value;
    // currentArgs.push_back(name);

    while(check(TokenType::WORD)) {
        currentArgs.push_back(advance().value);
    }

    if (auto cmd = CommandRegistry::getCmd(name))
    {
        // std::cout<<"WE GOT comm COMMANDREGISTRY"<<std::endl;
        if (auto builtinCmd = dynamic_cast<InBuiltCmd*>(cmd))
        {
            auto cmdPtr = std::shared_ptr<InBuiltCmd>(builtinCmd, [](InBuiltCmd*) {
                /* Empty deleter - we don't own the pointer */
            });
            
            if (cmdPtr) {
                cmdPtr->setArgs(currentArgs);
                return cmdPtr;
            }
        }
    }


    std::string fullPath=getENVPath(name);
    if(fullPath!="")
    {
        pid_t pid=fork();
        if(pid==-1)perror("fork failed");
        else if(pid==0)
        {
            std::vector<char*> argV;
            for(std::string&arg : currentArgs)argV.push_back(const_cast<char*>(arg.c_str()));
            argV.push_back(nullptr);
            execv(fullPath.c_str(),argV.data());
        }
        else
        {
            int status;
            waitpid(pid,&status,0);
        }
    }
    else
    {
        std::cout<<name<<": command not found"<<std::endl;
    }


    return nullptr;
}

std::shared_ptr<Command> Parser::parseCommand() {
    return parseSimpleComm();
}

std::shared_ptr<Command> Parser::parse()
{
    if(isAtEnd()) return nullptr;
    return parseCommand();
}
