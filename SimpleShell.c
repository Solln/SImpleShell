/******************************************************************************
@file         SimgpleShell.c
@author       Darren Lowe, Jacqueline Catlin, Georgia Greegoriou, Cameron Turner, Ethan Dickson
@date         Thursday, 11/02/2016
@brief
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
int executeLine(char** params);
char* getPath(void);
int setPath( char*);
*/
#define MAXIN 512
#define MAXPARA 50
#define MAXPATH 120

const char* currentDir;
char* path;

char* getPath() { 
    printf("%s\n", getenv("PATH"));
    return path;
}

int setPath(char *newPath) {
    
    if (setenv("PATH", newPath, 1) != 0) return 1;
    return 0;
}

int executeLine(char** strings) {
        // Fork process
        pid_t pid = fork();

        // Error
        if (pid < 0) {
            char* error = strerror(errno);
            printf("fork: %s\n", error);
            return 1;
        }

        // Child process
        else if (pid == 0) {
            // Execute command
            execvp(strings[0], strings);

            // Error occurred
            perror(strings[0]);
            return 0;
        }

        // Parent process
        else {
            // Wait for child process to finish
            wait(NULL);
            return 1;
        }
}

int main()
{
    path = getenv("PATH");

    char line[MAXIN];
    char delim[] = " |><,&;\t";
    char* token;
    char* strings[MAXPARA];
    memset(strings, 0, sizeof(strings));
    char filePath[MAXPATH];
    chdir(getenv("HOME"));

    bool exitShell = false;
    while(!exitShell) {

	currentDir = (getcwd( filePath, MAXPATH ) != NULL)? filePath : "ERROR";
        printf("%s>", currentDir);

        if (fgets(line, sizeof(line), stdin) == NULL) break;
        (line[strlen(line)-1] == '\n')? line[strlen(line)-1] = '\0' : 0;

        token = strtok(line, delim);
        int i = 0;
        while (token != NULL){
            strings[i++] = token;
            //printf("token = %s\n", strings[i++]);
            token = strtok(NULL, delim);
        }
	if (strings[0] == NULL) continue;
        if ((strcmp("exit", strings[0]) == 0))  {                    
                    exitShell = true;
                    //break;
        }
        else if (strcmp("getpath", strings[0]) == 0) {
            getPath();
        }
        else if (strcmp("setpath", strings[0]) == 0) {
            if (setPath(strings[1])) perror("setenv error:");
        }
	       
        //Code for the cd command, needs to change the directory name when showing the path.
         
        else if (strcmp("cd", strings[0]) == 0){
            if (strings[1] == NULL) {
                chdir(getenv("HOME"));
            } else {
               if (chdir(strings[1]) != 0) {
                   perror("error");
               }
            }
        }

        else executeLine(strings);

        //To flush strings at the end of each cycle of input
        memset(strings, 0, sizeof(strings));
   }
   setPath(path);
   getPath();
   printf("Quitting\n");
   return 0;
}

/*

   Need to error trap for White space input (can trap other error input here as well)
   Need to add the exit command to break the loop
    Added 14/02/2016
*/
