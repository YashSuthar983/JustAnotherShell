#include "InBuilt.h"
#include "Global.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <limits.h>


extern char cwd[PATH_MAX];
extern std::string getENVPath(std::string& cmdName);
extern std::string getNextToken(std::vector<std::string>& tokens);
extern size_t curr;


std::map<std::string,std::unique_ptr<Command>> CommandRegistry::map;

std::string EchoCommand::execute()
{
    std::stringstream output;
    for(size_t i = 1; i < args.size(); i++) {
        output << args[i];
        if(i < args.size() - 1) output << " ";
    }
    output << "\n";
    return output.str();
}

std::string ExitCommand::execute() {
    if(args.size() <= 1) exit(0);
    try {
        int value = std::stoi(args[1]);
        exit(value);
    } catch(const std::exception& e) {
        return "Only integer return code expected\n";
    }
    return "";
}

std::string TypeCommand::execute()
{
    std::string temp="";
    std::string code = getNextToken(args);
    Command* cmd = CommandRegistry::getCmd(code);
    if(code == "")
    {
        return "NULL";
    }
    if(cmd != nullptr)
    {
        return code+cmd->cmdType();
    }

    std::string fullPath = getENVPath(code);
    if(fullPath != "")
    {
        temp += code + " is " + fullPath;
        return temp;
    }
    
    return code + ": not found" ;
}

std::string PwdCommand::execute()
{
    return (std::string) cwd ;
}

std::string CdCommand::execute()
{
    if(args.empty())
    {
        return "WE GOT THE ARGS EMPTY";
    }
    std::string intendedPath = args[0];
    std::cout<<"WE GOT THE NEXT TOKEN : "<<intendedPath<<std::endl;
    if(intendedPath == "~")
    {
        chdir(getenv("HOME"));
    }
    else if(chdir(intendedPath.c_str()) == -1)
    {
        std::cout << "cd: " << intendedPath << ": No such file or directory" << std::endl;
    }
    getcwd(cwd, sizeof(cwd));
    return "";
}

void setupCommands() {
    REGISTERCMD("echo", EchoCommand);
    REGISTERCMD("exit", ExitCommand);
    REGISTERCMD("type", TypeCommand);
    REGISTERCMD("pwd", PwdCommand);
    REGISTERCMD("cd", CdCommand);
}
