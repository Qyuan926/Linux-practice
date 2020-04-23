#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdlib.h>

char g_Command[1024];

int GetCommand()
{
   memset(g_Command, '\0', sizeof(g_Command)); 
   printf("[Qyuan@localhost minshell]$");
   fflush(stdout);
   if(fgets(g_Command, sizeof(g_Command) - 1, stdin) == NULL)
   {
       printf("fgets error\n");
       return -1;
   }
   return 0;
}


int ExecCommand(char* argv[])
{
    if(argv[0] == NULL)
    {
        printf("ExecCommand error\n");
        return -1;
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return -1;
    }
    else if(pid == 0)
    {
 
        execvp(argv[0], argv); 

        exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    return 0;
}

int DealCommand(char* command)
{
    if(!command && *command == '\0')
    {
        printf("command error\n");
        return -1;
    }
    int argc = 0;
    char* argv[1024] = {0};

    while(*command)
    {
        while(!isspace(*command) && *command != '\0')
        {
            argv[argc] = command;
            argc++;
           
            while(!isspace(*command) && *command != '\0')
            {
                command++;
            }
            *command = '\0';
        }
        command++;
    }
    argv[argc] = NULL;
    ExecCommand(argv);
    return 0;
}
int main()
{
   
    while(1)
    {
       if(GetCommand() == -1)
       {
           continue;
       }
    
       DealCommand(g_Command);
    }
    return 0;
}
