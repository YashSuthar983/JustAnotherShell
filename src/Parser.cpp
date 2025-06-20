#include "Parser.h"
#include "Token.h"
#include "DebuggerLog.h"
#include "Ast.h"
#include "Global.h"
#include <sys/wait.h>
#include <unistd.h>
#include <memory>
#include <type_traits>

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
    currentArgs.push_back(name);  // Include command name as first argument

    while(check(TokenType::WORD)) {
        currentArgs.push_back(advance().value);
    }
    
    DB("Parsing Simple Command.");
    if (auto cmd = CommandRegistry::getCmd(name)) {
        cmd->setArgs(currentArgs);
        cmd->name = name;
        cmd->isBuiltIn = true;
        DB("parsed Simple Command || got inbuilt command.");
        return cmd;
    }
    
    // For external commands, create a SimpleCommand
    auto cmd = std::make_shared<SimpleCommand>(name, currentArgs);
    cmd->name = name;
    DB("Created external command");
    return cmd;
}

std::shared_ptr<Command> Parser::parseRedirectionComm()
{
    std::shared_ptr<Command> cmd=parseSimpleComm();
    if(!cmd)return nullptr;
    
    if(check(TokenType::REDIRECT_OUT)||check(TokenType::REDIRECT_IN))
    {
        int file_mode = O_TRUNC;
        Token redirect=advance();

        if(!(check(TokenType::WORD)))
        {
            std::cerr << "Syntax error: Expected filename after redirection token '" << redirect.getType() << "'" << std::endl;
            return nullptr;
        }
        Token file_name =advance();
        int redirect_mode=(redirect.type==REDIRECT_IN)?0:1;
        int fd_to_redirect = (redirect.type == TokenType::REDIRECT_IN) ? STDIN_FILENO : STDOUT_FILENO;
        return std::make_shared<RedirectCommand>(cmd,file_name.value,fd_to_redirect,redirect_mode,file_mode);
    }
    else if(check(TokenType::REDIRECT_IN_APP)||check(TokenType::REDIRECT_OUT_APP))
    {
        int file_mode = O_APPEND;
        Token redirect=advance();

        if(!(check(TokenType::WORD)))
        {
            std::cerr << "Syntax error: Expected filename after redirection token '" << redirect.getType() << "'" << std::endl;
            return nullptr;
        }
        Token file_name =advance();
        int redirect_mode=(redirect.type==REDIRECT_IN_APP)?0:1;
        int fd_to_redirect = (redirect.type == TokenType::REDIRECT_IN_APP) ? STDIN_FILENO : STDOUT_FILENO;
        return std::make_shared<RedirectCommand>(cmd,file_name.value,fd_to_redirect,redirect_mode,file_mode);
    }
    else if(check(TokenType::IO_NUMBER))
    {
        int file_mode = O_TRUNC;
        Token tok = advance();
        if(!(check(TokenType::REDIRECT_IN_APP)||check(TokenType::REDIRECT_OUT_APP)||check(TokenType::REDIRECT_OUT)||check(TokenType::REDIRECT_IN))) {
            std::cerr << "Syntax error: Expected IO_NUMBER token, found WORD '" << tok.value << "'" << std::endl;
            return nullptr;
        }
        std::string redirect = advance().value;
        if (redirect.size() < 1) {
            std::cerr << "Syntax error: Invalid IO_NUMBER token '" << redirect << "'" << std::endl;
            return nullptr;
        }
        int fd = tok.value[0] - '0';
        int redirect_mode = (redirect.back() == '<') ? 0 : 1;
        // Check for double redirection (e.g., "2>>" or "1<<")
        if (redirect.size() >= 2) {
            std::string last_two = redirect.substr(redirect.size() - 2, 2);
            if (last_two == "<<") redirect_mode = 0;
            else if (last_two == ">>") {redirect_mode = 1;file_mode=O_APPEND;}
        }
        if (!(check(TokenType::WORD))) {
            std::cerr << "Syntax error: Expected filename after redirection token '" << tok.getType() << "'" << std::endl;
            return nullptr;
        }
        Token file_name = advance();
        return std::make_shared<RedirectCommand>(cmd, file_name.value, fd, redirect_mode,file_mode);
    }
    return cmd;
}

std::shared_ptr<Command> Parser::parsePipeComm()
{
    std::shared_ptr<Command> leftInputCom=parseRedirectionComm();
    std::shared_ptr<Command> rightInputCom;
    
    while(check(TOKEN_PIPE))
    {
        advance();
        rightInputCom=parseRedirectionComm();
        leftInputCom= std::make_shared<PipeCommand>(leftInputCom,rightInputCom);
        DB("Parsing Pipe Command.")
    }
    return leftInputCom;
}

std::shared_ptr<Command> Parser::parseCommand() {
    return parsePipeComm();
}

std::shared_ptr<Command> Parser::parse()
{
    if(isAtEnd()) return nullptr;
    return parseCommand();
}
