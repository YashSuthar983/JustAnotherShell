#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <limits.h>

// Global variables
extern char cwd[PATH_MAX];
// extern int indTok;
extern size_t curr;

// Global functions
std::string getNextToken(std::vector<std::string>& tokens);
std::string getENVPath(std::string& cmdName);
