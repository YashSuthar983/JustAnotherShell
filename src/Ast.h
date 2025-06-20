#pragma once 
#include <string>
#include <vector>
#include <memory>
#include "InBuilt.h"
#include <fcntl.h>

struct SimpleCommand :Command
{
    SimpleCommand(std::string name, std::vector<std::string> args) {
        this->args = args;
        this->name = name;
        this->isBuiltIn = false;
    }
    void execute() override ;
    std::string cmdType() override { return " is an external command\n"; }
    virtual ~SimpleCommand() = default;
};

struct RedirectCommand : Command
{
    RedirectCommand(std::shared_ptr<Command> cmd,std::string target,int fd,int redirection_mode,int file_mode=O_TRUNC)
        :   cmd(cmd),fd(fd),target(target),file_mode(file_mode){this->name="Redirection";}
    std::shared_ptr<Command> cmd;
    int fd;
    int redirection_mode; // 1 for output redirection, 0 for input redirection
    int file_mode;
    std::string target;
    void execute() override ;
    std::string cmdType() override { return " is an external command\n"; }
    virtual ~RedirectCommand()=default;
};

struct PipeCommand : Command
{
    PipeCommand(std::shared_ptr<Command>& cmdinput,std::shared_ptr<Command>& cmdoutput)
        :   cmdinput(cmdinput),cmdoutput(cmdoutput){this->name="Pipe";}
    std::shared_ptr<Command> cmdinput;
    std::shared_ptr<Command> cmdoutput;
    void execute() override ;
    std::string cmdType() override { return " is an external command\n"; }
    virtual ~PipeCommand()=default;
};
