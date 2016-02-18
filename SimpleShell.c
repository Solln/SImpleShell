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
//for fork()
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>


#define MAXIN 512
#define MAXPARA 50
#define INERR -1

char filePath[] = "C:\\";

int setenv() {
    printf("Current filepath is %s\n",filePath);
}

int main() {
char line [MAXIN];
char delim[] = " \t|><,&;";
char *token;
char *tokens[MAXPARA];
bool exitShell = false;

pid_t pid;
int *ret_status;

while (exitShell == false) {
    do{
        printf("%s>", filePath);
        fgets(line, MAXIN, stdin);
    } while (strlen(line) <= 1);
    (line[strlen(line)-1] == '\n')? line[strlen(line)-1] = '\0' : 0;
    token = strtok(line, delim);
    int i = 0;
    for (token = strtok(line, delim); tokens[i++] = token; token = strtok(NULL, delim));
    i = 0;
    for(i = 0; i < 3; i++)
        printf("token = (%s)\n", tokens[i]);
    if ((strcmp("exit", tokens[0]) == 0))  {
        printf("Quitting\n");
        exitShell = true;
    }
    }
    return 0;
}

/* Moving onto task two, checked about adding the fork and wait statement. but It broke the exit command.
 Entered this code in where its marked ***Here***


 else

{
    pid = fork();
    if (pid){
        printf("Waiting for child (%d)\n", pid);
        pid = wait(ret_status);
        printf("Child (%d) finished\n", pid);
    }
}

Had these variables set as well at the top:

 pid_t pid;
 int *ret_status;

As well as these includes:
 #include <unistd.h>
 #include <errno.h>
 #include <sys/types.h>

Unsure as to exact implementation, was using http://rik0.altervista.org/snippets/csimpleshell.html as advice.




   Need to error trap for White space input (can trap other error input here as well)
   Need to add the exit command to break the loop
    Added 14/02/2016
*/
