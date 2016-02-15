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

#define MAXIN 512
#define INERR -1


int main() {
char line [MAXIN];
char delim[] = " |><,&;";
char* token;
bool exitShell = false;

    while (exitShell == false) {

        do{
            printf(">");
            fgets(line, MAXIN, stdin);
        } while (strlen(line) <= 1);
        token = strtok(line, delim);
        while (token != NULL){
            if ((strcmp("exit\n", token) == 0) || (strcmp("<ctrl>-D\n", token) == 0))  {
                    printf("Quitting\n");
                    exitShell = true;
                    //return 1;
            }
            printf("token = %s\n", token);
            token = strtok(NULL, delim);
        }
    }
    return 0;
}

/* Need to error trap for White space input (can trap other error input here as well)
   Need to add the exit command to break the loop
    Added 14/02/2016
*/
