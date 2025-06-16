#include "Global.h"
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

// Define global variables
char cwd[PATH_MAX];
size_t curr ;

std::string getNextToken(std::vector<std::string>& tokens) {
    if(curr < tokens.size()) return tokens[curr++];
    return "";
}

std::string getENVPath(std::string& cmdName) {
    const char* pathEnv = std::getenv("PATH");
    if(pathEnv != nullptr) {
        std::string pathStr(pathEnv);
        std::stringstream pathSS(pathEnv);
        std::string dir;
        struct stat sb;
        while (std::getline(pathSS, dir, ':')) {
            if(dir.empty()) continue;
            std::string fullPath;

            if(dir.back() == '/') fullPath = dir + cmdName;
            else fullPath = dir + '/' + cmdName;
            
            if(access(fullPath.c_str(), X_OK) == 0 && stat(fullPath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
                return fullPath;
            }
        }
    }
    return "";
}
