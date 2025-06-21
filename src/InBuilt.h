#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>

#define REGISTERCMD(NAME,TYPE) CommandRegistry::registerCmd(NAME, std::make_shared<TYPE>())

class Command {
public:
    std::string name;
    std::vector<std::string> args;
    virtual void execute() = 0;
    virtual std::string cmdType() = 0;
    bool isBuiltIn = false;
    void setArgs(const std::vector<std::string>& commandArgs) {
        args = commandArgs;
    }
    virtual ~Command() = default;
};

class CommandRegistry {
public:
    static std::map<std::string,std::shared_ptr<Command>> map;
    static void registerCmd(std::string name, std::shared_ptr<Command> ptr) { map[name] = std::move(ptr); }
    static std::shared_ptr<Command> getCmd(std::string& name) {
        if(map.find(name) != map.end()) return map[name];
        return nullptr;
    }
};

class InBuiltCmd : public Command {
public:
    bool isBuiltIn = true;
    std::string cmdType() override {
        return " is a shell builtin\n";
    }

    virtual ~InBuiltCmd() = default;
};

class EchoCommand : public InBuiltCmd {
public:
    void execute() override;
};

class ExitCommand : public InBuiltCmd {
public:
    void execute() override;
};

class TypeCommand : public InBuiltCmd {
public:
    void execute() override;
};

class PwdCommand : public InBuiltCmd {
public:
    void execute() override;
};

class CdCommand : public InBuiltCmd {
public:
    void execute() override;
};

class HistoryCommand : public InBuiltCmd {
public:
    void execute() override;
};

void setupCommands();