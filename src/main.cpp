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
#include "Executer.h"
#include "DebuggerLog.h"

bool debug=false;
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

int main(int argc, char* argv[])
{
    if(argc == 2 && (std::string)argv[1] == "debug") {
        debug = true;
    }
    
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    std::string input;

    getcwd(cwd, sizeof(cwd));
    setupCommands();

    while(true) {
        std::cout << "$ ";
        curr = 0;
        
        if(!std::getline(std::cin, input)) {
            // Handle EOF (Ctrl+D)
            std::cout << std::endl;
            break;
        }
        
        if(input.empty()) {
            continue;
        }
        
        DB("Input: " + input);
        
        Scanner scanner(input);
        std::vector<Token> tokens = scanner.scanToken();
        
        if(tokens.empty()) {
            DB("No tokens found");
            continue;
        }
        
        DB("Token count: " + std::to_string(tokens.size()));
        for(auto& token : tokens) {
            DB("Token: " + token.value +" : Type "+token.getType());
        }
        
        Parser parser(tokens);
        std::shared_ptr<Command> cmd = parser.parse();

        if(cmd) {
            DB("Executing command: " + cmd->name + " with " + 
               std::to_string(cmd->args.size()) + " arguments");
            interpret(cmd);
            // std::cout << output;
        } else {
            DB("Failed to parse command");
        }
    }
    
    return 0;
}