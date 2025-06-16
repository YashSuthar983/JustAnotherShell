#pragma once 
#include <string>
#include <vector>
#include <memory>

#include "InBuilt.h"

struct SimpleCommand :Command
{
    SimpleCommand(std::string comm, std::vector<std::string> args) : comm(comm){this->args=args;}
    std::vector<std::string> args;
    std::string comm;
    std::string execute() override;
};

struct RedirectCommand : Command
{
    RedirectCommand(std::shared_ptr<Command> cmd,int f,std::string target)
        :   cmd(cmd),f(f),target(target) {}
    std::shared_ptr<Command> cmd;
    int f;
    std::string target;
    std::string execute() override;
};
