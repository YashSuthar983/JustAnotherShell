#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>

#define REGISTERCMD(NAME,TYPE) CommandRegistry::registerCmd(NAME, std::make_unique<TYPE>())

class Command {    
public:
    virtual std::string execute() = 0;
    virtual std::string cmdType() = 0;
    virtual ~Command() = default;
};

class CommandRegistry {
public:
    static std::map<std::string,std::unique_ptr<Command>> map;
    static void registerCmd(std::string name, std::unique_ptr<Command> ptr) { map[name] = std::move(ptr); }
    static Command* getCmd(std::string& name) {
        if(map.find(name) != map.end()) return map[name].get();
        return nullptr;
    }
};

class InBuiltCmd : public Command {
protected:
    std::vector<std::string> args;
public:
    void setArgs(const std::vector<std::string>& commandArgs) {
        args = commandArgs;
    }

    std::string cmdType() override {
        return " is a shell builtin\n";
    }

    virtual ~InBuiltCmd() = default;
};

class EchoCommand : public InBuiltCmd {
public:
    std::string execute() override;
};

class ExitCommand : public InBuiltCmd {
public:
    std::string execute() override;
};

class TypeCommand : public InBuiltCmd {
public:
    std::string execute() override;
};

class PwdCommand : public InBuiltCmd {
public:
    std::string execute() override;
};

class CdCommand : public InBuiltCmd {
public:
    std::string execute() override;
};

void setupCommands();