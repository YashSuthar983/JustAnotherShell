#pragma once 
#include <memory>
#include "Ast.h"
#include "Global.h"
#include <unistd.h>
#include <sys/wait.h>
void executePathComm(std::string&name,std::vector<std::string>&args);
void interpret(const std::shared_ptr<Command>&cmd);