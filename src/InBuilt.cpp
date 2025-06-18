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


std::map<std::string,std::shared_ptr<Command>> CommandRegistry::map;


void setupCommands() {
    REGISTERCMD("echo", EchoCommand);
    REGISTERCMD("exit", ExitCommand);
    REGISTERCMD("type", TypeCommand);
    REGISTERCMD("pwd", PwdCommand);
    REGISTERCMD("cd", CdCommand);
}
