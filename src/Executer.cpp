#include "Executer.h"
#include "DebuggerLog.h"
#include <fcntl.h>
#include "ReadlineCus/ReadlineCus.h"
#include <string.h>
#include "Global.h"

void executePathComm(std::string&name,std::vector<std::string>&args)
{
    std::string fullPath=getENVPath(name);

    pid_t pid =fork();
    if (pid==-1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        if(fullPath!="")
        {
            std::vector<char*> argV;
            for(std::string&arg : args)argV.push_back(const_cast<char*>(arg.c_str()));
            argV.push_back(nullptr);
            DB("Executing command: " + fullPath + " with arguments: " + std::to_string(argV.size()));
            execv(fullPath.c_str(),argV.data());
            perror(("execv failed for " + name).c_str());
            exit(127);
        }
        else
        {
            std::cout<<name<<": command not found"<<std::endl;
            exit(127);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

void EchoCommand::execute()
{
    executePathComm(name,args);
}

void ExitCommand::execute() {
    if(args.size() <= 1) exit(0);
    try {
        int value = std::stoi(args[1]);
        DB("Exiting with code: " + std::to_string(value));
        writeHistoryOnStartup();
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
    executePathComm(name,args);
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

void PipeCommand::execute()
{
    DB("PIP COMMAND EXECUTED with "<<cmdinput->name<<"->"<<cmdoutput->name)
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork 1");
        return;
    }
    
    if (pid1 == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        
        cmdinput->execute();
        exit(EXIT_SUCCESS);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork 2");
        return;
    }

    if (pid2 == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        
        interpret(cmdoutput);
        exit(EXIT_SUCCESS);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    DB("PIP COMMAND EXECUTED SUCCESFULLY");
}

void HistoryCommand::execute()
{
    // if(args.size() < 2)
    // {
    //     std::cout << "Usage: history <number_of_commands>\n";
    //     return;
    // }
    if(args.size()==3)
    {
        if(args[1]=="-r")
        {
            readHistory(args[2]);
        }
        else if(args[1]=="-w"||args[1]=="-a")
        {
            writeOrAppHistory(args[1],args[2]);
        }
        return;
    }
    
    int num = commandHistory.size();

    if(args.size()==2)
    {
        num = std::stoi(args[1]);
        if(num <= 0)
        {
            std::cout << "Number of commands should be greater than 0.\n";
            return;
        }
    }
    for(int i = commandHistory.size() - num; i < commandHistory.size(); ++i)
    {
        if(i >= 0) {
            std::cout <<"\t"<<i+1<<"  "<< commandHistory[i] << "\n";
        }
    }  
}

void interpret(const std::shared_ptr<Command>& cmd)
{
    if(std::dynamic_pointer_cast<PipeCommand>(cmd)||std::dynamic_pointer_cast<RedirectCommand>(cmd))cmd->execute();
    else cmd->execute();
}