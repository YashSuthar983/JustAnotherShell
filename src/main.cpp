/*              access 
| Flag   | Meaning                         |
| ------ | ------------------------------- |
| `F_OK` | Check if the file exists        |
| `R_OK` | Check if file is **readable**   |
| `W_OK` | Check if file is **writable**   |
| `X_OK` | Check if file is **executable** |
*/


#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <vector>
#include <filesystem>
#include <limits.h>
#include "Scanner.h"
#include "InBuilt.h"
#include "Global.h"
#include "Parser.h"

enum ReDirectionType
{
  ReInput,
  ReOutput_Trun,
  ReOutput_App
};

struct ReDirection
{
  ReDirectionType type;
  int id;
  std::string target;
};

int main()
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    std::string input;

    getcwd(cwd,sizeof(cwd));
    setupCommands();

    while(true)
    {
      std::cout << "$ ";

      curr=0;
      std::getline(std::cin, input);
      if(input.empty())continue;      Scanner scanner(input);
      std::vector<Token> Tokens = scanner.scanToken();
      Parser parser(Tokens);

      for(auto&x:Tokens)std::cout<<x.value<<" ";
      std::cout<<std::endl;

      std::shared_ptr<Command> cmd = parser.parse();

      if(cmd)
      {
        std::string output = cmd->execute();
        std::cout << output;
      }


      // std::vector<std::string> tokens=ParseCommand(input);

      // std::string headCommand= getNextToken(tokens);

      // Command* cmd=CommandRegistry::getCmd(headCommand);
      // if(cmd!=nullptr)cmd->execute(tokens);
      // else
      // {
      //   std::string fullPath=getENVPath(headCommand);
      //   if(fullPath!="")
      //   {
      //       pid_t pid=fork();
      //       if(pid==-1)perror("fork failed");
      //       else if(pid==0)
      //       {
      //         std::vector<char*> argV;

      //         for(std::string&arg : tokens)argV.push_back(const_cast<char*>(arg.c_str()));
      //         argV.push_back(nullptr);
      //         execv(fullPath.c_str(),argV.data());
      //       }
      //       else
      //       {
      //         int status;
      //         waitpid(pid,&status,0);
      //       }
      //   }
      //   else
      //   {
      //     std::cout<<headCommand<<": command not found"<<std::endl;
      //   }
      // }

    }
}