// closh.c - COSC 315, Winter 2020
// YOUR NAME HERE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0

// tokenize the command string into arguments - do not modify
void readCmdTokens(char* cmd, char** cmdTokens) {
    cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
    int i = 0;
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens)) {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// read one character of input, then discard up to the newline - do not modify
char readChar() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

// main method - program entry point
int main() {
    char cmd[81]; // array of chars (a string)
    char* cmdTokens[20]; // array of strings
    int count; // number of times to execute command
    int parallel; // whether to run in parallel or sequentially
    int timeout; // max seconds to run set of commands (parallel) or each command (sequentially)
    pid_t pid;
    
    while (TRUE) { // main shell input loop
        
        // begin parsing code - do not modify
        printf("closh> ");
        fgets(cmd, sizeof(cmd), stdin);
        if (cmd[0] == '\n') continue;
        readCmdTokens(cmd, cmdTokens);
        do {
            printf("  count> ");
            count = readChar() - '0';
        } while (count <= 0 || count > 9);
        
        printf("  [p]arallel or [s]equential> ");
        parallel = (readChar() == 'p') ? TRUE : FALSE;
        do {
            printf("  timeout> ");
            timeout = readChar() - '0';
        } while (timeout < 0 || timeout > 9);
        
        // end parsing code
        
        
        ////////////////////////////////////////////////////////
        //                                                    //
        // TODO: use cmdTokens, count, parallel, and timeout  //
        // to implement the rest of closh                     //
        //                                                    //
        // /////////////////////////////////////////////////////
        int i,pid;
        pid_t pids[count];
        
        if (parallel == FALSE)
        {
        	for(i = 0; i<count; i++)
        	{
			//fork 
			pid = fork();
			//execvp
			if (pid == 0)
			{
				printf("Process ID is %d \n", getpid());
				execvp(cmdTokens[0], cmdTokens);
				printf("Can't execute %s\n", cmdTokens[0]);
				exit(1);
			}
			else
			{
				waitpid(pid,NULL,0);
				sleep(timeout);
				kill(pid, SIGKILL);
			}
        	};
        }
        else
        {
        	for (i = 0; i < count; i++)
        	{
        		pids[i] = fork();
        		if (pids[i] == 0)
        		{
        			printf("Process ID is %d \n", getpid());
        			execvp(cmdTokens[0], cmdTokens);
				printf("Can't execute %s\n", cmdTokens[0]);
				exit(1);
        		}
        	}
        	if (timeout > 0)
		{
			sleep(timeout);
			for (i = 0; i < count; i++)
			{
				kill(pids[i], SIGKILL);
			}
		}
        }
   
    }
}

