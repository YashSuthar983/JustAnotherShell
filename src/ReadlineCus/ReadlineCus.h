#pragma once

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <dirent.h>

extern struct termios orig_termios;

extern bool history_bool;
extern int comhisPos;
extern std::vector<std::string> commandHistory;

void enableHistory();
void disableHistory();
void readHistory(const std::string&str);
void writeOrAppHistory(const std::string&str,const std::string&path);

void writeHistoryOnStartup();

void addCmdToHistory(std::string& str);
void loadHistoryOnStartup();

void disableRawMode();
void enableRawMode();
std::string readlineCus();

std::vector<std::string> findCompliation(std::string& check);
std::vector<std::string> findAllCommands();
std::vector<std::string> split(std::string&str,char del);