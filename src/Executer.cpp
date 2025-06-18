#include "Executer.h"
#include "DebuggerLog.h"
#include <fcntl.h>

void EchoCommand::execute()
{
    std::stringstream output;
    for(size_t i = 1; i < args.size(); i++) {
        output << args[i];
        if(i < args.size() - 1) output << " ";
    }
    output << "\n";
    std::cout<< output.str();
}

void ExitCommand::execute() {
    if(args.size() <= 1) exit(0);
    try {
        int value = std::stoi(args[1]);
        exit(value);
    } catch(const std::exception& e) {
        std::cerr<<"Only integer return code expected\n";
    }
}

void TypeCommand::execute()
{
    std::string temp="";
    if(args.size() < 2) {
        std::cout << "Usage: type <command>\n";
        return;
    }
    std::string code = args[1];
    std::shared_ptr<Command> cmd = CommandRegistry::getCmd(code);
    if(cmd != nullptr)
    {
        std::cout<<code+cmdType();
        return;
    }

    std::string fullPath = getENVPath(code);
    if(fullPath != "")
    {
        temp += code + " is " + fullPath;
        std::cout<<temp<<std::endl;
        return;
    }
    
    std::cout<<code + ": not found"<<std::endl; ;
}

void PwdCommand::execute()
{
    std::cout<<(std::string) cwd<<std::endl;
}

void CdCommand::execute()
{
    if(args.size() < 2) {
        // CD with no arguments should go to home directory
        const char* home = getenv("HOME");
        if (home) {
            chdir(home);
            getcwd(cwd, sizeof(cwd));
        }
        return ;
    }
    
    std::string intendedPath = args[1];  // args[0] is "cd", args[1] is the path
    
    if(intendedPath == "~")
    {
        const char* home = getenv("HOME");
        if (home)
        {
            chdir(home);
        }
    }
    else if(chdir(intendedPath.c_str()) == -1)
    {
        std::cout << "cd: " << intendedPath << ": No such file or directory" << std::endl;
    }
    
    getcwd(cwd, sizeof(cwd));
}

void SimpleCommand::execute()
{
    std::string fullPath=getENVPath(name);

    if(fullPath!="")
    {
        pid_t pid=fork();
        if(pid==-1)perror("fork failed");
        else if(pid==0)
        {
            std::vector<char*> argV;
            for(std::string&arg : args)argV.push_back(const_cast<char*>(arg.c_str()));
            argV.push_back(nullptr);
            DB("Executing command: " + fullPath + " with arguments: " + std::to_string(argV.size()));
            execv(fullPath.c_str(),argV.data());
        }
        else
        {
            int status;
            waitpid(pid,&status,0);
        }
    }
    else
    {
        std::cout<<name<<": command not found"<<std::endl;
    }
}

void RedirectCommand::execute()
{
    pid_t pid=fork();
    if(pid==0)
    {
        DB("Interpreting the RedirectCommand.("+target+")")
        int file_fd;
        int fd_toReplcae;
        if(redirection_mode==0)
        {
            file_fd=open(target.c_str(),O_WRONLY|O_CREAT|file_mode,0666);
        }
        else
        {
            file_fd=open(target.c_str(),O_RDONLY);
        }
        if(file_fd==-1)
        {
            std::cerr << "Error opening file: " << target << std::endl;
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(file_fd,fd);
        close(file_fd);
        cmd->execute();
        exit(EXIT_SUCCESS);
    }
    else
    {
        int status;
        waitpid(pid,&status,0);
    }
}

void interpret(const std::shared_ptr<Command>& cmd)
{
    cmd->execute();
}