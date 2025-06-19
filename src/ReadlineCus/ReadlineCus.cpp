#include "ReadlineCus.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include "../DebuggerLog.h"
#include <sstream>
#include <set>

struct termios orig_termios;
bool history_bool=false;
int comhisPos=0;
std::vector<std::string> commandHistory;

void enableHistory(){history_bool=true;comhisPos=0;}
void disableHistory(){history_bool=false;}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

std::vector<std::string> split(std::string& str,char del)
{
    std::vector<std::string> pathvec;
    std::stringstream ss(str);
    std::string tok;
    while (std::getline(ss,tok,del))
    {
        pathvec.push_back(tok);
    }
    return pathvec;
}

std::vector<std::string> findCompliation(std::string& check)
{
    std::vector<std::string> matches;
    DIR* dir;
    struct dirent* dirp;
    if((dir=opendir(".")))
    {
        while (dirp=readdir(dir))
        {
            std::string filename=dirp->d_name;
            if(filename.find(check,0)==0)matches.push_back(filename);
        }
        closedir(dir);
    }
    for(const auto&x:findAllCommands())if(x.find(check,0)==0)matches.push_back(x);
    return matches;
}

std::vector<std::string> findAllCommands()
{
    std::string path=getenv("PATH");
    std::vector<std::string> pathvec=split(path,':');
    std::set<std::string> allcommands;
    for(auto&x:pathvec)
    {
        DIR* dir;
        struct dirent* dirp;
        if((dir=opendir(x.c_str())))
        {
            while (dirp=readdir(dir))
            {
                allcommands.insert(dirp->d_name);
            }
            closedir(dir);
        }
    }
    return std::vector<std::string>(allcommands.begin(),allcommands.end());
}


std::string readlineCus()
{
    std::string str="";
    size_t currPos=0;
    std::string tempstr="";
    while(true)
    {
        std::cout << "\r\x1b[K";
        std::cout<<"\r$ "<<str<<"\x1b[k";
        std::cout<<"\r\x1b["<<(currPos+2)<<"C";
        std::cout.flush();

        char c;
        if(read(STDIN_FILENO,&c,1)!=1)continue;

        if(c=='\x1b')
        {
            char seq[3];
            if(read(STDIN_FILENO,&seq[0],1)!=1) continue;
            if(read(STDIN_FILENO,&seq[1],1)!=1)continue;

            if(seq[0]=='[')
            {
                switch(seq[1])
                {
                    case 'A':
                            DB("Looking upward in CommandHistory");
                            if (!commandHistory.empty() && comhisPos < (int)commandHistory.size()) {
                                ++comhisPos;
                                str = *(commandHistory.end() - comhisPos);
                                currPos = str.length();
                            }
                            break;
                    case 'B':
                            DB("Looking downward in CommandHistory");
                            if (comhisPos > 1) {
                                --comhisPos;
                                str = *(commandHistory.end() - comhisPos);
                                currPos = str.length();
                            } else if (comhisPos == 1) {
                                --comhisPos;
                                str = tempstr;
                                currPos = str.length();
                            }
                            break;
                    case 'C':
                            if(currPos<str.length())currPos++;
                            break;
                    case 'D':
                            if(currPos>0)currPos--;
                            break;
                }
            }
        }
        else if(c==127||c==8)
        {
            if(currPos>0)
            {
                str.erase(currPos-1,1);
                currPos--;
            }
        }
        else if(c=='\t')
        {
            size_t word_st=str.find_last_of(" \t\n",currPos-1);
            if(word_st!=std::string::npos)word_st=0;
            else word_st++;

            std::string str_to_comp=str.substr(word_st,currPos-word_st);

            std::vector<std::string> matches=findCompliation(str_to_comp);

            if(matches.size()==1)
            {
                std::string comp=matches[0].substr(str_to_comp.size());
                str.insert(currPos,comp);
                str+=' ';
                currPos+=comp.size()+1;
            }
            else if(matches.size()>1)
            {
                std::string prefix = matches[0];
                for(auto&x:matches)
                {
                    int j=0;
                    while (j<x.size()&& prefix.size()&& prefix[j]==x[j])j++;
                    prefix=prefix.substr(0,j);
                }
                if(prefix.size()>str_to_comp.size())
                {
                    std::string temp=prefix.substr(str_to_comp.size());
                    str.insert(currPos,temp);
                    currPos+=temp.size();
                }
                else
                {
                    std::cout<<std::endl;
                    for(auto&x:matches)
                    {
                        std::cout<<x<<"\t";
                    }
                    std::cout<<std::endl;
                }
            }
        }
        else if(c=='\n'||c=='\r')
        {
            std::cout<<std::endl;
            if(history_bool)commandHistory.push_back(str);
            DB("Adding : "+str+" : to the history")
            return str;
        }
        else if(c==3)
        {
            std::cout<<"^C"<<std::endl;
            return "";
        }
        else if(c==4)
        {
            if(str.empty())
            {
                std::cout<<"exit"<<std::endl;
                exit(0);
            }
        }
        else if(isprint(c))
        {
            str.insert(currPos,1,c);
            currPos++;
            tempstr=str;
        }
        
    }
}
